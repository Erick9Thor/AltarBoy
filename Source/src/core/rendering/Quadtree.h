#pragma once

#include "core/GameObject.h"
#include "components/ComponentMeshRenderer.h"

#include <MathGeoLib.h>
#include <unordered_set>

#define QUADTREE_MAX_ITEMS 16
#define QUADTREE_MIN_SIZE 50.f

namespace Hachiko
{
    enum class Quadrants
    {
        NW = 0,
        NE,
        SE,
        SW,
        COUNT
    };

    class QuadtreeNode
    {
    public:
        QuadtreeNode(const AABB& box, QuadtreeNode* parent, int depth);
        ~QuadtreeNode();

        void Insert(const std::unordered_set<ComponentMeshRenderer*>& to_insert);
        void Remove(const std::unordered_set<ComponentMeshRenderer*>& to_remove);
        void CreateChildren();
        void RearangeChildren();

        [[nodiscard]] bool IsLeaf() const
        {
            return children[0] == nullptr;
        }

        [[nodiscard]] const AABB& GetBox() const
        {
            return box;
        }

        [[nodiscard]] const std::vector<ComponentMeshRenderer*>& GetMeshes() const
        {
            return meshes;
        }

        template<typename T>
        void GetIntersections(std::vector<GameObject*>& objects, const T& primitive) const;

        void GetIntersections(std::vector<ComponentMeshRenderer*>& objects, const Frustum& frustum) const;

        QuadtreeNode* children[static_cast<int>(Quadrants::COUNT)] {};

        void DebugDraw();

    private:
        int depth = 0;
        AABB box;
        QuadtreeNode* parent = nullptr;
        std::vector<ComponentMeshRenderer*> meshes {};
    };

    class Quadtree
    {
    public:
        Quadtree();
        ~Quadtree();

        void Clear();
        void SetBox(const AABB& box);

        void Insert(ComponentMeshRenderer* mesh);
        void Remove(ComponentMeshRenderer* mesh);
        void Refresh();

        [[nodiscard]] QuadtreeNode* GetRoot() const
        {
            return root;
        }

        void DebugDraw() const;

    private:
        std::unordered_set<ComponentMeshRenderer*> to_remove = {};
        std::unordered_set<ComponentMeshRenderer*> to_insert = {};
        QuadtreeNode* root = nullptr;
    };

    template<typename T>
    void QuadtreeNode::GetIntersections(std::vector<GameObject*>& intersected, const T& primitive) const
    {
        if (primitive.Intersects(box))
        {
            float near_hit, far_hit;
            for (ComponentMeshRenderer* mesh : meshes)
            {
                if (primitive.Intersects(mesh->GetOBB(), near_hit, far_hit))
                {
                    intersected.push_back(mesh->GetGameObject());
                }
            }

            // If it has one child all exist
            if (children[0] != nullptr)
            {
                for (int i = 0; i < 4; ++i)
                {
                    children[i]->GetIntersections(intersected, primitive);
                }
            }
        }
    }
} // namespace Hachiko