#pragma once

#include "MathGeoLib.h"

#include <vector>

namespace Hachiko
{
    class GameObject;
    class ComponentMesh;
    class ComponentCamera;
    class QuadtreeNode;

    struct RenderTarget
    {
        const char* name = nullptr;
        GameObject* game_object = nullptr;
        ComponentMesh* mesh = nullptr;
        float distance = 0.0f;
    };

    class RenderList
    {
    public:
        void Update(ComponentCamera* camera, GameObject* game_object);
        void Update(ComponentCamera* camera, QuadtreeNode* quadtree);

        std::vector<RenderTarget>& GetNodes()
        {
            return nodes;
        }

        [[nodiscard]] const std::vector<RenderTarget>& GetNodes() const
        {
            return nodes;
        }

    private:
        void CollectObjects(ComponentCamera* camera, const float3& camera_pos, GameObject* game_object);
        void CollectObjects(ComponentCamera* camera, const float3& camera_pos, QuadtreeNode* quadtree);
        void CollectMesh(const float3& camera_pos, GameObject* game_object);

        std::vector<RenderTarget> nodes;
    };
}
