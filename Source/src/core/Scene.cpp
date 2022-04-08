#include "core/hepch.h"
#include "Scene.h"

#include "components/ComponentTransform.h"
#include "components/ComponentCamera.h"
#include "components/ComponentMesh.h"
#include "components/ComponentMaterial.h"
#include "components/ComponentImage.h"

#include "modules/ModuleTexture.h"
#include "modules/ModuleEditor.h"
#include "modules/ModuleCamera.h"
#include "modules/ModuleDebugDraw.h"
#include "modules/ModuleResources.h"

#include "resources/ResourceModel.h"
#include "resources/ResourceMaterial.h"

Hachiko::Scene::Scene():
    root(new GameObject(nullptr, float4x4::identity, "Root")),
    culling_camera(App->camera->GetMainCamera()),
    skybox(new Skybox()),
    quadtree(new Quadtree()),
    name(UNNAMED_SCENE)
{
    // TODO: Send hardcoded values to preferences
    quadtree->SetBox(AABB(float3(-500, 0, -500), float3(500, 250, 500)));
}

Hachiko::Scene::~Scene()
{
    CleanScene();
}

void Hachiko::Scene::CleanScene() const
{
    App->editor->SetSelectedGO(nullptr);
    delete root;
    delete skybox;
    delete quadtree;
}

void Hachiko::Scene::DestroyGameObject(GameObject* game_object) const
{
    if (App->editor->GetSelectedGO() == game_object)
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
    GameObject* game_object = App->editor->GetSelectedGO();
    if (game_object == nullptr)
    {
        game_object = CreateNewGameObject(nullptr, model->model_name.c_str());
    }

    std::function<void(GameObject*, const std::vector<ResourceNode*>&)> createChilds = [&](GameObject* parent, const std::vector<ResourceNode*>& childs)
    {
        for (auto child : childs)
        {
            GameObject* last_parent = parent;

            if (!child->meshes_index.empty())
            {
                last_parent = CreateNewGameObject(parent, child->node_name.c_str());

                for (unsigned i = 0; i < child->meshes_index.size(); ++i)
                {
                    UID mesh_id = model->meshes[child->meshes_index[i]].mesh_id;
                    ComponentMesh* component = static_cast<ComponentMesh*>(last_parent->CreateComponent(Component::Type::MESH));
                    component->SetID(mesh_id); // TODO: ask if this is correct (i dont think so)
                    component->SetResourcePath(model->model_path);
                    component->SetModelName(model->model_name);

                    component->SetMeshIndex(child->meshes_index[i]); // the component mesh support one mesh so we take the first of the node
                    component->AddResourceMesh(App->resources->GetMesh(mesh_id));
                }

                ComponentMaterial* component_material = static_cast<ComponentMaterial*>(last_parent->CreateComponent(Component::Type::MATERIAL));
                component_material->SetResourceMaterial(App->resources->GetMaterial(child->material_name));
            }
            
            createChilds(last_parent, child->childs);
        }
    };

    createChilds(game_object, model->child_nodes);
}

//Hachiko::GameObject* Hachiko::Scene::LoadImageObject(const std::string& path)
//{
//    const auto file_name = path.substr(path.find_last_of("/\\") + 1);
//    const auto name = file_name.substr(0, file_name.find_last_of('.'));
//
//    GameObject* game_object = nullptr;
//    game_object = CreateNewGameObject(name.c_str(), root);
//    game_object->CreateComponent(Component::Type::TRANSFORM_2D);
//    game_object->CreateComponent(Component::Type::CANVAS_RENDERER);
//    game_object->CreateComponent(Component::Type::IMAGE);
//    ComponentImage* image = game_object->GetComponent<ComponentImage>();
//    image->Import(path.c_str());
//
//    return game_object;
//}

void Hachiko::Scene::HandleInputMaterial(ResourceMaterial* material)
{
    GameObject* game_object = App->editor->GetSelectedGO();
    if (game_object == nullptr)
    {
        HE_LOG("No game object selected to apply material on");
        return;
    }

    ComponentMaterial* component_material = game_object->GetComponent<ComponentMaterial>();
    if (component_material != nullptr)
    {
        component_material->SetResourceMaterial(material);
    }
}

Hachiko::GameObject* Hachiko::Scene::RayCast(const float3& origin, const float3& destination) const
{
    LineSegment line_seg(origin, destination);
    return RayCast(line_seg);
}

Hachiko::GameObject* Hachiko::Scene::RayCast(const LineSegment& segment) const
{
    GameObject* selected = nullptr;
    float closest_hit_distance = inf;

    std::vector<GameObject*> game_objects;
    quadtree->GetRoot()->GetIntersections(game_objects, segment);

    for (GameObject* game_object : game_objects)
    {
        auto* mesh = game_object->GetComponent<ComponentMesh>();
        if (!mesh)
        {
            continue;
        }
        // Transform ray to mesh space, more efficient
        LineSegment local_segment(segment);
        local_segment.Transform(game_object->GetTransform()->GetMatrix().Inverted());

        // This is done always for the first mesh in ComponentMesh
        const float* vertices = mesh->GetVertices(0);
        const unsigned* indices = mesh->GetIndices(0);
        for (unsigned i = 0; i < mesh->GetBufferSize(0, ResourceMesh::Buffers::INDICES); i += 3)
        {
            float hit_distance;
            float3 hit_point;
            if (!local_segment.Intersects(triangle, &hit_distance, &hit_point))
            {
                continue;
            }
            if (hit_distance < closest_hit_distance)
            {
                closest_hit_distance = hit_distance;
                selected = game_object;
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
    const YAML::Node childs_node = node[CHILD_NODE];
    for (unsigned i = 0; i < childs_node.size(); ++i)
    {
        std::string child_name = childs_node[i][GAME_OBJECT_NAME].as<std::string>();
        UID child_uid = childs_node[i][GAME_OBJECT_ID].as<unsigned long long>();
        const auto child = new GameObject(root, child_name.c_str(), child_uid);
        child->scene_owner = this;
        child->Load(childs_node[i]);
    }
}

void Hachiko::Scene::CreateLights()
{
    GameObject* sun = CreateNewGameObject(root , "Sun");
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
}
