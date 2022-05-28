#include "core/hepch.h"
#include "RenderList.h"

#include "components/ComponentCamera.h"
#include "components/ComponentMeshRenderer.h"

void Hachiko::RenderList::PreUpdate()
{
    polycount_rendered = 0;
    polycount_total = 0;
}

void Hachiko::RenderList::Update(ComponentCamera* camera, QuadtreeNode* quadtree)
{
    opaque_targets.clear();
    transparent_targets.clear();
    const Frustum* frustum = camera->GetFrustum();
    const float3 camera_pos = frustum->WorldMatrix().TranslatePart();
    CollectMeshes(camera, camera_pos, quadtree);
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

void Hachiko::RenderList::CollectMesh(const float3& camera_pos, ComponentMeshRenderer* mesh_renderer)
{
    if (!mesh_renderer || !mesh_renderer->IsVisible())
    {
        return;
    }

    GameObject* game_object = mesh_renderer->GetGameObject();
    
    RenderTarget target;
    target.name = game_object->GetName().c_str();
    target.mesh_renderer = mesh_renderer;
    target.distance = (mesh_renderer->GetOBB().CenterPoint() - camera_pos).LengthSq();

    // Decide in which list this target should be placed in based on its
    // material's transparency:
    auto targets = mesh_renderer->GetResourceMaterial()->is_transparent 
        ? &transparent_targets 
        : &opaque_targets;

    // Get sorted by distance to camera in ascending order:
    const auto it = std::lower_bound(
        (*targets).begin(), (*targets).end(), target, 
        [](const RenderTarget& it_target, const RenderTarget& new_target) 
        { 
            return it_target.distance < new_target.distance; 
        });

    (*targets).insert(it, target);
}
