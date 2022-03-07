#include "core/hepch.h"
#include "Scene.h"

#include "components/ComponentTransform.h"
#include "components/ComponentCamera.h"
#include "components/ComponentMesh.h"
#include "components/ComponentMaterial.h"

#include "modules/ModuleTexture.h"
#include "modules/ModuleEditor.h"
#include "modules/ModuleCamera.h"
#include "modules/ModuleDebugDraw.h"

Hachiko::Scene::Scene():
    root(new GameObject(nullptr, float4x4::identity, "Root")),
    culling_camera(App->camera->GetMainCamera()),
    skybox(new Skybox()),
    quadtree(new Quadtree()),
    name(UNNAMED_SCENE)
{
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
    if (App->editor->GetSelectedGameObject() == game_object)
    {
        App->editor->SetSelectedGO(nullptr);
    }
    quadtree->Remove(game_object);
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

Hachiko::GameObject* Hachiko::Scene::RayCast(const LineSegment& segment) const
{
    GameObject* selected = nullptr;
    float closest_hit_distance = inf;

    std::vector<GameObject*> game_objects;
    quadtree->GetRoot()->GetIntersections(game_objects, segment);

    for (GameObject* game_object : game_objects)
    {
        ComponentMesh* mesh = game_object->GetComponent<ComponentMesh>();
        if (!mesh)
        {
            continue;
        }
        // Transform ray to mesh space, more efficient
        LineSegment local_segment(segment);
        local_segment.Transform(game_object->GetComponent<ComponentTransform>()->GetTransform().Inverted());

        // TODO: This check is only for the first mesh in the model.
        // Check if the model has meshes loaded
        const float* vertices = mesh->GetVertices(0);
        const unsigned* indices = mesh->GetIndices(0);
        for (unsigned i = 0; i < mesh->GetBufferSize(0, ResourceMesh::Buffers::INDICES); i += 3)
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
    return selected;
}

void Hachiko::Scene::CreateLights()
{
    GameObject* sun = CreateNewGameObject(root , "Sun");
    sun->GetComponent<ComponentTransform>()->SetLocalPosition(float3(1, 1, -1));
    sun->GetComponent<ComponentTransform>()->LookAt(float3(0, 0, 0));
    sun->CreateComponent(Component::Type::DIRLIGHT);

    GameObject* spot = CreateNewGameObject(root, "Spot Light");
    spot->GetComponent<ComponentTransform>()->SetLocalPosition(float3(-1, 1, -1));

    spot->CreateComponent(Component::Type::SPOTLIGHT);

    GameObject* point = CreateNewGameObject(root, "Point Light");
    point->GetComponent<ComponentTransform>()->SetLocalPosition(float3(0, 1, -1));
    point->CreateComponent(Component::Type::POINTLIGHT);
}

Hachiko::GameObject* Hachiko::Scene::CreateDebugCamera()
{
    GameObject* camera = CreateNewGameObject(root, "Debug Camera");
    camera->GetComponent<ComponentTransform>()->SetLocalPosition(float3(5, 5, 0));
    camera->CreateComponent(Component::Type::CAMERA);
    camera->GetComponent<ComponentTransform>()->LookAt(float3(0, 5, 0));

    auto* debug_camera = camera->GetComponent<ComponentCamera>();
    debug_camera->SetFarPlane(100.0f);
    debug_camera->draw_frustum = true;

    return camera;
}

void Hachiko::Scene::Update() const
{
    root->Update();
}
