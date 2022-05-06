#include "core/hepch.h"
#include "Scene.h"

#include "components/ComponentTransform.h"
#include "components/ComponentCamera.h"
#include "components/ComponentMesh.h"
#include "components/ComponentMaterial.h"
#include "components/ComponentImage.h"
#include "components/ComponentAnimation.h"

#include "modules/ModuleTexture.h"
#include "modules/ModuleEditor.h"
#include "modules/ModuleCamera.h"
#include "modules/ModuleDebugDraw.h"
#include "modules/ModuleResources.h"

#include "resources/ResourceModel.h"
#include "resources/ResourceMaterial.h"
#include <debugdraw.h>

//#include "imgui_node_editor.h"
#include "imnodes.h"
#include "resources/ResourceStateMachine.h"

//namespace ed = ax::NodeEditor;
//static ed::EditorContext* g_Context = nullptr;
bool addNode = false;
bool addClip = false;
bool deleteClip = false;
bool deleteNode = false;
int nodeId = 0;
bool started = false;
int linkId = 0;
bool deleteLink = false;
Hachiko::ResourceStateMachine sm = Hachiko::ResourceStateMachine(1);

Hachiko::Scene::Scene() :
    root(new GameObject(nullptr, float4x4::identity, "Root")),
    culling_camera(App->camera->GetMainCamera()),
    skybox(new Skybox()),
    quadtree(new Quadtree()),
    loaded(false),
    name(UNNAMED_SCENE)
{
    // TODO: Send hardcoded values to preferences
    quadtree->SetBox(AABB(float3(-500, -100, -500), float3(500, 250, 500)));

    //ed::Config config;
    //config.SettingsFile = "Simple.jasn";
    //g_Context = ed::CreateEditor(&config);
    sm.AddState("Idle", "clip1");
    sm.AddState("Walking", "clip2");
    sm.AddState("Running", "clip3");
    sm.AddState("Cry", "clip4");
    sm.AddTransition("Idle", "Walking", "t", 1);
    sm.AddTransition("Walking", "Running", "t", 1);
    ImNodes::CreateContext();
}

Hachiko::Scene::~Scene()
{
    //ed::DestroyEditor(g_Context);
    ImNodes::DestroyContext();

    CleanScene();
}

void Hachiko::Scene::CleanScene()
{
    App->editor->SetSelectedGO(nullptr);
    delete root;
    delete skybox;
    delete quadtree;
    loaded = false;
}

void Hachiko::Scene::DestroyGameObject(GameObject* game_object) const
{
    if (App->editor->GetSelectedGameObject() == game_object)
    {
        App->editor->SetSelectedGO(nullptr);
    }
    quadtree->Remove(game_object);
}

Hachiko::ComponentCamera* Hachiko::Scene::GetMainCamera() const
{
    return SearchMainCamera(root);
}

Hachiko::ComponentCamera* Hachiko::Scene::SearchMainCamera(GameObject* game_object) const
{
    ComponentCamera* component_camera = nullptr;
    component_camera = game_object->GetComponent<ComponentCamera>();
    if (component_camera != nullptr)
    {
        return component_camera;
    }

    for (GameObject* child : game_object->children)
    {
        component_camera = SearchMainCamera(child);
        if (component_camera != nullptr)
        {
            return component_camera;
        }
    }
    return nullptr;
}

void Hachiko::Scene::AddGameObject(GameObject* new_object, GameObject* parent) const
{
    GameObject* new_parent = parent ? parent : root;
    new_parent->children.push_back(new_object);
    quadtree->Insert(new_object);
}

Hachiko::GameObject* Hachiko::Scene::CreateNewGameObject(GameObject* parent, const char* name)
{
    // It will insert itself into quadtree on first bounding box update
    const auto game_object = new GameObject(parent ? parent : root, name);
    game_object->scene_owner = this;
    return game_object;
}

void Hachiko::Scene::HandleInputModel(ResourceModel* model)
{
    GameObject* game_object = CreateNewGameObject(nullptr, model->model_name.c_str());

    std::function<void(GameObject*, const std::vector<ResourceNode*>&)> create_children_function = [&](GameObject* parent, const std::vector<ResourceNode*>& children) {
        for (auto child : children)
        {
            GameObject* last_parent = parent;

            last_parent = CreateNewGameObject(parent, child->node_name.c_str());

            if (!child->meshes_index.empty())
            {
                for (unsigned i = 0; i < child->meshes_index.size(); ++i)
                {
                    MeshInfo mesh_info = model->meshes[child->meshes_index[i]];
                    ComponentMesh* component = static_cast<ComponentMesh*>(last_parent->CreateComponent(Component::Type::MESH));
                    component->SetID(mesh_info.mesh_id); // TODO: ask if this is correct (i dont think so)
                    component->SetResourcePath(model->model_path);
                    component->SetModelName(model->model_name);

                    component->SetMeshIndex(child->meshes_index[i]); // the component mesh support one mesh so we take the first of the node
                    component->AddResourceMesh(App->resources->GetMesh(mesh_info.mesh_id));

                    ComponentMaterial* component_material = static_cast<ComponentMaterial*>(last_parent->CreateComponent(Component::Type::MATERIAL));
                    component_material->SetResourceMaterial(App->resources->GetMaterial(model->materials[mesh_info.material_index].material_name));
                }
            }

            last_parent->GetComponent<ComponentTransform>()->SetLocalTransform(child->node_transform);

            create_children_function(last_parent, child->children);
        }
    };

    create_children_function(game_object, model->child_nodes);
}

void Hachiko::Scene::HandleInputMaterial(ResourceMaterial* material)
{
    GameObject* game_object = App->editor->GetSelectedGameObject();
    if (game_object == nullptr)
    {
        HE_LOG("No game object selected to apply a material on");
        return;
    }

    ComponentMaterial* component_material = game_object->GetComponent<ComponentMaterial>();
    if (component_material != nullptr)
    {
        component_material->SetResourceMaterial(material);
    }
}

Hachiko::GameObject* Hachiko::Scene::Raycast(const float3& origin, const float3& destination) const
{
    LineSegment line_seg(origin, destination);
    return Raycast(line_seg);
}

Hachiko::GameObject* Hachiko::Scene::Raycast(const LineSegment& segment) const
{
    GameObject* selected = nullptr;
    float closest_hit_distance = inf;

    std::vector<GameObject*> game_objects;
    quadtree->GetRoot()->GetIntersections(game_objects, segment);

    for (GameObject* game_object : game_objects)
    {
        auto* mesh = game_object->GetComponent<ComponentMesh>();
        if (mesh)
        {
            // Transform ray to mesh space, more efficient
            LineSegment local_segment(segment);
            local_segment.Transform(game_object->GetTransform()->GetGlobalMatrix().Inverted());

            const float* vertices = mesh->GetVertices();
            const unsigned* indices = mesh->GetIndices();
            for (unsigned i = 0; i < mesh->GetBufferSize(ResourceMesh::Buffers::INDICES); i += 3)
            {
                Triangle triangle;
                triangle.a = vec(&vertices[indices[i] * 3]);
                triangle.b = vec(&vertices[indices[i + 1] * 3]);
                triangle.c = vec(&vertices[indices[i + 2] * 3]);

                float hit_distance;
                float3 hit_point;
                if (local_segment.Intersects(triangle, &hit_distance, &hit_point))
                {
                    if (hit_distance < closest_hit_distance)
                    {
                        closest_hit_distance = hit_distance;
                        selected = game_object;
                    }
                }
            }
        }
    }
    return selected;
}

void Hachiko::Scene::Save(YAML::Node& node) const
{
    node[SCENE_NAME] = GetName();
    node[ROOT_ID] = GetRoot()->GetID();
    for (int i = 0; i < GetRoot()->children.size(); ++i)
    {
        GetRoot()->children[i]->Save(node[CHILD_NODE][i]);
    }
}

void Hachiko::Scene::Load(const YAML::Node& node)
{
    SetName(node[SCENE_NAME].as<std::string>().c_str());
    root->SetID(node[ROOT_ID].as<UID>());
    const YAML::Node children_node = node[CHILD_NODE];

    for (unsigned i = 0; i < children_node.size(); ++i)
    {
        std::string child_name = children_node[i][GAME_OBJECT_NAME].as<std::string>();
        UID child_uid = children_node[i][GAME_OBJECT_ID].as<UID>();
        const auto child = new GameObject(root, child_name.c_str(), child_uid);
        child->scene_owner = this;
        child->Load(children_node[i]);
    }

    loaded = true;
}

void Hachiko::Scene::CreateLights()
{
    GameObject* sun = CreateNewGameObject(root, "Sun");
    sun->GetTransform()->SetLocalPosition(float3(1, 1, -1));
    sun->GetTransform()->LookAtTarget(float3(0, 0, 0));
    sun->CreateComponent(Component::Type::DIRLIGHT);

    GameObject* spot = CreateNewGameObject(root, "Spot Light");
    sun->GetTransform()->SetLocalPosition(float3(-1, 1, -1));

    spot->CreateComponent(Component::Type::SPOTLIGHT);

    GameObject* point = CreateNewGameObject(root, "Point Light");
    sun->GetTransform()->SetLocalPosition(float3(0, 1, -1));
    point->CreateComponent(Component::Type::POINTLIGHT);
}

Hachiko::GameObject* Hachiko::Scene::CreateDebugCamera()
{
    GameObject* camera = CreateNewGameObject(root, "Debug Camera");
    camera->GetTransform()->SetLocalPosition(float3(5, 5, 0));
    camera->CreateComponent(Component::Type::CAMERA);
    camera->GetTransform()->LookAtTarget(float3(0, 5, 0));

    auto* debug_camera = camera->GetComponent<ComponentCamera>();
    debug_camera->SetFarPlane(100.0f);
    debug_camera->draw_frustum = true;

    return camera;
}

void Hachiko::Scene::Start() const
{
    root->Start();
}

void Hachiko::Scene::Update() const
{
    root->Update();

    struct Node
    {
        std::string name;
        int nodeIndex;
        int inputIndex;
        int outputIndex;

        Node() {};
        Node(const std::string& name, int nodeIndex, int inputIndex, int outputIndex) : name(name), nodeIndex(nodeIndex), inputIndex(inputIndex), outputIndex(outputIndex) {};
    };
    struct Link
    {
        int id;
        int from;
        int to;

        Link() {};
        Link(int id, int from, int to) : id(id), from(from), to(to) {};
    };

    std::vector<Node> nodes;
    std::vector<Link> links;
    

    // Show node editor ------------------------------------------------------------------
    ImNodes::SetNodeGridSpacePos(0, ImVec2(200.0f, 200.0f));
    ImGui::Begin("Node editor");
    ImNodes::BeginNodeEditor();
    
    for (int i = 0; i < sm.states.size(); ++i)
    {
        int id = i * 3;
        ImNodes::BeginNode(id + 1);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(sm.states[i].name.c_str());
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(id + 2);
        ImGui::Text(sm.states[i].clip.c_str());
        ImNodes::EndInputAttribute();

        ImNodes::BeginOutputAttribute(id + 3);
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();
        nodes.push_back(Node(sm.states[i].name.c_str(), id + 1, id + 2, id + 3));
    }

    for (int i = 0; i < sm.transitions.size(); ++i)
    {
        int start, end;
        for (int j = 0; j < nodes.size(); ++j)
        {
            if (sm.transitions[i].source == nodes[j].name)
            {
                start = nodes[j].outputIndex;
            }
            if (sm.transitions[i].target == nodes[j].name)
            {
                end = nodes[j].inputIndex;
            }
        }
        ImNodes::Link(i + 1, start, end);
        links.push_back(Link(i + 1, start, end));
    }
    // End of show node editor ------------------------------------------------------------------

    // Show popup to create a node ------------------------------------------------------------------
    const bool openPopup = ImNodes::IsEditorHovered() && ImGui::IsMouseClicked(1, false);

    if (!ImGui::IsAnyItemHovered() && openPopup)
    {
        ImGui::OpenPopup("AddNode");
    }


    if (ImGui::BeginPopup("AddNode"))
    {
        if (ImGui::Button(" Add node "))
        {
            addNode = true;
        }

        if (addNode)
        {
            static char nodeName[128] = "";
            const ImGuiInputTextFlags nodeName_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText(" Node name ", nodeName, IM_ARRAYSIZE(nodeName), nodeName_input_flags))
            {
                addNode = false;
                sm.AddState(nodeName, "");
            }
        }

        ImGui::EndPopup();
    }

    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    ImNodes::EndNodeEditor();
    
    // End of show popup to create a node ------------------------------------------------------------------
    






    // Show popup to add a clip ------------------------------------------------------------------
    for (int i = 0; i < nodes.size(); ++i)
    {
        if (ImNodes::IsNodeHovered(&i) && ImGui::IsMouseClicked(1, false))
        {
            nodeId = i;
            ImGui::OpenPopup("EditNode");
        }
    }
 
    if (ImGui::BeginPopup("EditNode"))
    {
        if (ImGui::Button(" Add clip "))
        {
            addClip = true;
        }
        else if (ImGui::Button(" Delete clip "))
        {
            deleteClip = true;
        }
        else if (ImGui::Button(" Delete node "))
        {
            deleteNode = true;
        }

        if (addClip)
        {
            static char clipName[128] = "";
            const ImGuiInputTextFlags nodeName_input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText(" Clip name ", clipName, IM_ARRAYSIZE(clipName), nodeName_input_flags))
            {
                addClip = false;
                for (int i = 0; i < nodes.size(); ++i)
                {
                    if (nodes[i].nodeIndex == nodeId)
                    {
                        sm.EditStateClip(nodes[i].name, clipName);
                    }
                }
            }
            ImGui::CloseCurrentPopup();
        }
        else if (deleteClip)
        {
            deleteClip = false;
            for (int i = 0; i < nodes.size(); ++i)
            {
                if (nodes[i].nodeIndex == nodeId)
                {
                    sm.EditStateClip(nodes[i].name, "");
                }
            }
            ImGui::CloseCurrentPopup();
        }
        else if (deleteNode)
        {
            deleteNode = false;
            for (int i = 0; i < nodes.size(); ++i)
            {
                if (nodes[i].nodeIndex == nodeId)
                {
                    sm.RemoveState(nodes[i].name);
                    nodes.erase(nodes.begin() + i);
                }
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    // End of show popup to add a clip ------------------------------------------------------------------
    
    

    // Add a link ------------------------------------------------------------------
    int start, end;
    for (int i = 0; i < nodes.size(); ++i)
    {
        int pin = (i + 1) * 3;
        if (ImNodes::IsLinkStarted(&pin))
        {
            start = pin;
            started = true;
        }
    }

    for (int i = 0; i < nodes.size(); ++i)
    {
        int end = i + 2;
        if (started && ImNodes::IsLinkCreated(&start, &end, false))
        {
            std::string startName, endName;
            for (int j = 0; j < nodes.size(); ++j)
            {
                if (nodes[j].outputIndex == start)
                {
                    startName = nodes[j].name;
                }
                if (nodes[j].inputIndex == end)
                {
                    endName = nodes[j].name;
                }
            }
            sm.AddTransition(startName, endName, "", 3);
        }
    }
    // End of add a link ------------------------------------------------------------------

    
    // Delete a link ------------------------------------------------------------------
    for (int i = 0; i < links.size(); ++i)
    {
        if (ImNodes::IsLinkHovered(&links[i].id) && ImGui::IsMouseClicked(1, false))
        {
            linkId = links[i].id;
            ImGui::OpenPopup("DeleteLink");
        }
    }
    
    if (ImGui::BeginPopup("DeleteLink"))
    {
        if (ImGui::Button(" Delete link "))
        {
            deleteLink = true;
        }

        if (deleteLink)
        {
            deleteLink = false;
            for (int i = 0; i < nodes.size(); ++i)
            {
                int a = links[linkId - 1].from;
                int b = nodes[i].outputIndex;
                if (links[linkId - 1].from == nodes[i].outputIndex)
                {
                    sm.RemoveTransition(nodes[i].name, "t");
                    links.erase(links.begin() + linkId - 1);
                    break;
                }
            }
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    
    ImGui::End();
}