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
    opaque_targets.clear();
    transparent_targets.clear();
    const Frustum* frustum = camera->GetFrustum();
    const float3 camera_pos = frustum->WorldMatrix().TranslatePart();
    CollectObjects(camera, camera_pos, game_object);
}

void Hachiko::RenderList::Update(ComponentCamera* camera, QuadtreeNode* quadtree)
{
    opaque_targets.clear();
    transparent_targets.clear();
    const Frustum* frustum = camera->GetFrustum();
    const float3 camera_pos = frustum->WorldMatrix().TranslatePart();
    CollectObjects(camera, camera_pos, quadtree);
}

void Hachiko::RenderList::CollectObjects(ComponentCamera* camera, const float3& camera_pos, GameObject* game_object)
{
    const bool inside = camera->GetFrustum()->Intersects(game_object->GetOBB());
    if (inside)
    {
        CollectMesh(camera_pos, game_object);
    }

    for (GameObject* child : game_object->children)
        CollectObjects(camera, camera_pos, child);
}

void Hachiko::RenderList::CollectObjects(ComponentCamera* camera, const float3& camera_pos, QuadtreeNode* quadtree)
{
    const Frustum* frustum = camera->GetFrustum();
    const bool quad_inside = frustum->Intersects(quadtree->GetBox());
    // Check if node intersects camera
    if (quad_inside)
    {
        // Check any gameobjects intersect
        for (GameObject* game_object : quadtree->GetObjects())
        {
            const bool object_inside = frustum->Intersects(game_object->GetOBB());

            if (object_inside)
            {
                CollectMesh(camera_pos, game_object);
            }
        }
        // Call for all children (What to do if it is duplicated when collecting)?
        if (!quadtree->IsLeaf())
        {
            for (QuadtreeNode* child : quadtree->children)
            {
                CollectObjects(camera, camera_pos, child);
            }
        }
    }
}

void Hachiko::RenderList::CollectMesh(const float3& camera_pos, 
    GameObject* game_object)
{
    std::vector<ComponentMeshRenderer*> component_mesh_renderers = 
        game_object->GetComponents<ComponentMeshRenderer>();
 
    for (int i = 0; i < component_mesh_renderers.size(); ++i)
    {
        ComponentMeshRenderer* mesh_renderer = 
            component_mesh_renderers[i];

        if (!mesh_renderer->IsVisible())
        {
            continue;
        }

        polycount_rendered += 
            mesh_renderer->GetBufferSize(ResourceMesh::Buffers::INDICES)/3;

        RenderTarget target;
        target.name = game_object->GetName().c_str();
        target.mesh_renderer = mesh_renderer;
        target.distance = 
            (game_object->GetOBB().CenterPoint() - camera_pos).LengthSq();

        // Decide in which list this target should be placed in based on its
        // material's transparency:
        auto targets = mesh_renderer->GetMaterial()->is_transparent 
            ? &transparent_targets : &opaque_targets;

        // Get sorted by distance to camera in ascending order:
        const auto it
            = std::lower_bound((*targets).begin(), (*targets).end(),
            target,
            [](const RenderTarget& it_target, const RenderTarget& new_target) 
            {
                return it_target.distance < new_target.distance;
            });

        (*targets).insert(it, target);
    }
}
