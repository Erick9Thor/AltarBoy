#pragma once

#include "core/GameObject.h"
#include "components/ComponentMeshRenderer.h"

#include <MathGeoLib.h>
#include <unordered_set>

#define QUADTREE_MAX_ITEMS 8
#define QUADTREE_MAX_DEPTH 8

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
        friend class Quadtree;

    private:
        QuadtreeNode(const AABB& box, QuadtreeNode* parent, int depth);
        ~QuadtreeNode();

        void Insert(const std::unordered_set<ComponentMeshRenderer*>& to_insert);
        void Remove(std::unordered_set<ComponentMeshRenderer*>& to_remove);
        void CreateChildren();
        void DeleteChildren();
        unsigned RearangeChildren();

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
        
        QuadtreeNode* children[static_cast<int>(Quadrants::COUNT)] {};

        void DebugDraw();

    private:
        template<typename T>
        void GetIntersections(std::vector<GameObject*>& objects, const T& primitive);

        template<typename T>
        void GetIntersections(std::vector<ComponentMeshRenderer*>& meshes, const T& primitive);

        void GetIntersections(std::vector<ComponentMeshRenderer*>& objects, const Frustum& frustum);

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

        void Reposition(ComponentMeshRenderer* mesh);
        void Remove(ComponentMeshRenderer* mesh);
        

        [[nodiscard]] QuadtreeNode* GetRoot()
        {
            if (root)
            {
                Refresh();
            }
            return root;
        }

        template<typename T>
        void GetIntersections(std::vector<GameObject*>& intersected, const T& primitive);

        template<typename T>
        void GetIntersections(std::vector<ComponentMeshRenderer*>& intersected, const T& primitive);

        void DebugDraw();

    private:
        void Refresh();
        std::unordered_set<ComponentMeshRenderer*> to_remove = {};
        std::unordered_set<ComponentMeshRenderer*> to_insert = {};
        QuadtreeNode* root = nullptr;
    };

    template<typename T>
    void Quadtree::GetIntersections(std::vector<GameObject*>& intersected, const T& primitive)
    {
        if (root)
        {
            Refresh();
            root->GetIntersections(intersected, primitive);
        }
        
    }

    template<typename T>
    void Quadtree::GetIntersections(std::vector<ComponentMeshRenderer*>& intersected, const T& primitive)
    {
        if (root)
        {
            Refresh();
            root->GetIntersections(intersected, primitive);
        }
    }

    template<typename T>
    void QuadtreeNode::GetIntersections(std::vector<GameObject*>& intersected, const T& primitive)
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

    template<typename T>
    void QuadtreeNode::GetIntersections(std::vector<ComponentMeshRenderer*>& intersected, const T& primitive)
    {
        if (primitive.Intersects(box))
        {
            float near_hit, far_hit;
            for (ComponentMeshRenderer* mesh : meshes)
            {
                if (primitive.Intersects(mesh->GetOBB(), near_hit, far_hit))
                {
                    intersected.push_back(mesh);
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