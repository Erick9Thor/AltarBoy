#include "core/hepch.h"
#include "RenderList.h"

#include "components/ComponentCamera.h"
#include "components/ComponentMeshRenderer.h"

void Hachiko::RenderList::PreUpdate()
{
    polycount_rendered = 0;
    polycount_total = 0;
}

void Hachiko::RenderList::Update(ComponentCamera* camera, GameObject* game_object)
{
    nodes.clear();
    const Frustum* frustum = camera->GetFrustum();
    const float3 camera_pos = frustum->WorldMatrix().TranslatePart();
    CollectMeshes(camera, camera_pos, game_object);
}

void Hachiko::RenderList::Update(ComponentCamera* camera, QuadtreeNode* quadtree)
{
    nodes.clear();
    const Frustum* frustum = camera->GetFrustum();
    const float3 camera_pos = frustum->WorldMatrix().TranslatePart();
    CollectMeshes(camera, camera_pos, quadtree);
}

void Hachiko::RenderList::CollectMeshes(ComponentCamera* camera, const float3& camera_pos, GameObject* game_object)
{
    CollectMesh(camera_pos, game_object);

    for (GameObject* child : game_object->children)
    {
        CollectMeshes(camera, camera_pos, child);
    }        
}

void Hachiko::RenderList::CollectMeshes(ComponentCamera* camera, const float3& camera_pos, QuadtreeNode* quadtree)
{
    const Frustum* frustum = camera->GetFrustum();
    const bool quad_inside = frustum->Intersects(quadtree->GetBox());
    // Check if node intersects camera
    if (quad_inside)
    {
        // Check any gameobjects intersect
        for (ComponentMeshRenderer* mesh : quadtree->GetMeshes())
        {
            CollectMesh(camera_pos, mesh);
        }
        // Call for all children (What to do if it is duplicated when collecting)?
        if (!quadtree->IsLeaf())
        {
            for (QuadtreeNode* child : quadtree->children)
            {
                CollectMeshes(camera, camera_pos, child);
            }
        }
    }
}

void Hachiko::RenderList::CollectMesh(const float3& camera_pos, GameObject* game_object)
{
    ComponentMeshRenderer* mesh_renderer = game_object->GetComponent<ComponentMeshRenderer>();
    if (!mesh_renderer || !mesh_renderer->IsVisible())
    {
        return;
    }
    CollectMesh(camera_pos, mesh_renderer);
}

void Hachiko::RenderList::CollectMesh(const float3& camera_pos, ComponentMeshRenderer* mesh)
{
    GameObject* game_object = mesh->GetGameObject();
    RenderTarget target;
    target.name = game_object->GetName().c_str();
    target.game_object = game_object;
    target.mesh = mesh;
    target.distance = (mesh->GetOBB().CenterPoint() - camera_pos).LengthSq();

    // Get first element which distance is not less than current target one
    const auto it = std::lower_bound(nodes.begin(), nodes.end(), target, [](const RenderTarget& it_target, const RenderTarget& new_target) { return it_target.distance < new_target.distance; });
    nodes.insert(it, target);
}
