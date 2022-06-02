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
        GameObject* game_object = nullptr;
        ComponentMeshRenderer* mesh = nullptr;
        float distance = 0.0f;
    };

    class RenderList
    {
    public:
        void PreUpdate();
        void Update(ComponentCamera* camera, QuadtreeNode* quadtree);

        std::vector<RenderTarget>& GetNodes()
        {
            return nodes;
        }

        [[nodiscard]] const std::vector<RenderTarget>& GetNodes() const
        {
            return nodes;
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

        std::vector<RenderTarget> nodes;
        unsigned polycount_rendered = 0, polycount_total = 0;
    };
}
