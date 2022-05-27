#pragma once

#include <MathGeoLib.h>

#include <vector>

namespace Hachiko
{
    class GameObject;
    class ComponentMeshRenderer;
    class ComponentCamera;
    class QuadtreeNode;

    struct RenderTarget
    {
        const char* name = nullptr;
        ComponentMeshRenderer* mesh_renderer = nullptr;
        float distance = 0.0f;
    };

    class RenderList
    {
    public:
        void PreUpdate();
        void Update(ComponentCamera* camera, QuadtreeNode* quadtree);

        std::vector<RenderTarget>& GetOpaqueTargets()
        {
            return opaque_targets;
        }

        std::vector<RenderTarget>& GetTransparentTargets()
        {
            return transparent_targets;
        }

        [[nodiscard]] unsigned GetPolycountRendered() const
        {
            return polycount_rendered;
        }

        [[nodiscard]] unsigned GetPolycountTotal() const
        {
            return polycount_total;
        }

    private:
        void CollectMeshes(ComponentCamera* camera, const float3& camera_pos, QuadtreeNode* quadtree);
        void CollectMesh(const float3& camera_pos, ComponentMeshRenderer* mesh);

        std::vector<RenderTarget> opaque_targets;
        std::vector<RenderTarget> transparent_targets;

        unsigned polycount_rendered = 0, polycount_total = 0;
    };
}
