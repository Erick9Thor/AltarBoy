#include "core/hepch.h"
#include "Quadtree.h"
#include <debugdraw.h>

Hachiko::QuadtreeNode::QuadtreeNode(const AABB& box, QuadtreeNode* parent, int depth) : depth(depth), box(box), parent(parent)
{
    children[static_cast<int>(Quadrants::NW)] = children[static_cast<int>(Quadrants::NE)] = children[static_cast<int>(Quadrants::SE)] = children[static_cast<int>(Quadrants::SW)] = nullptr;
}

Hachiko::QuadtreeNode::~QuadtreeNode()
{
    DeleteChildren();
}

void Hachiko::QuadtreeNode::Insert(const std::unordered_set<ComponentMeshRenderer*>& to_insert)
{
    for (ComponentMeshRenderer* mesh : to_insert)
    {
        meshes.push_back(mesh);
    }
}

void Hachiko::QuadtreeNode::Remove(std::unordered_set<ComponentMeshRenderer*>& to_remove)
{
    if (to_remove.empty())
    {
        return;
    }
    
    auto it = meshes.rbegin();
    while (it != meshes.rend())
    {
        ComponentMeshRenderer* mesh = *it;
        if (to_remove.find(mesh) == to_remove.end())
        {
            ++it;
            continue;
        }
        it = decltype(it)(meshes.erase((it + 1).base()));
        // If we only push back meshes when they interssect a single square we dont have duplicates
        // This means we can remove it once removed
        to_remove.erase(mesh);

        if (to_remove.empty())
        {
            return;
        }
    }
    
    if (!IsLeaf())
    {
        for (const auto& child : children)
        {
            child->Remove(to_remove);
        }
    }
}

void Hachiko::QuadtreeNode::CreateChildren()
{
    // Subdivide current box
    const auto size = float3(box.Size());
    const float3 center = box.CenterPoint();
    // Do not operate y, 2d
    const auto child_size = float3(0.5f * size.x, size.y, 0.5f * size.z);
    // Recalculate for each child
    float3 child_center(center);
    AABB child_box;

    int new_detph = depth + 1;

    // NW
    child_center.x = center.x - size.x * 0.25f;
    child_center.z = center.z + size.z * 0.25f;
    child_box.SetFromCenterAndSize(child_center, child_size);
    children[static_cast<int>(Quadrants::NW)] = new QuadtreeNode(child_box, this, new_detph);

    // NE
    child_center.x = center.x + size.x * 0.25f;
    child_center.z = center.z + size.z * 0.25f;
    child_box.SetFromCenterAndSize(child_center, child_size);
    children[static_cast<int>(Quadrants::NE)] = new QuadtreeNode(child_box, this, new_detph);

    // SE
    child_center.x = center.x + size.x * 0.25f;
    child_center.z = center.z - size.z * 0.25f;
    child_box.SetFromCenterAndSize(child_center, child_size);
    children[static_cast<int>(Quadrants::SE)] = new QuadtreeNode(child_box, this, new_detph);

    // SW
    child_center.x = center.x - size.x * 0.25f;
    child_center.z = center.z - size.z * 0.25f;
    child_box.SetFromCenterAndSize(child_center, child_size);
    children[static_cast<int>(Quadrants::SW)] = new QuadtreeNode(child_box, this, new_detph);
}

void Hachiko::QuadtreeNode::DeleteChildren()
{
    RELEASE(children[static_cast<int>(Quadrants::NW)]);
    RELEASE(children[static_cast<int>(Quadrants::NE)]);
    RELEASE(children[static_cast<int>(Quadrants::SE)]);
    RELEASE(children[static_cast<int>(Quadrants::SW)]);
}

unsigned Hachiko::QuadtreeNode::RearangeChildren()
{
    // No split because already satisfies size conditions
    if (IsLeaf() && meshes.size() < QUADTREE_MAX_ITEMS)
    {
        return meshes.size();
    }
    
    // No split due to minimum size
    if (depth >= (QUADTREE_MAX_DEPTH))
    {
        return meshes.size();
    }

    // Rearrange children
    if (meshes.size() >= QUADTREE_MAX_ITEMS)
    {
        if (IsLeaf())
        {
            CreateChildren();
        }
        auto it = meshes.rbegin();
        while (it != meshes.rend())
        {
            ComponentMeshRenderer* mesh = *it;
            int intersection_count = 0;
            bool intersects[static_cast<int>(Quadrants::COUNT)];
            for (unsigned i = 0; i < static_cast<unsigned>(Quadrants::COUNT); ++i)
            {
                intersects[i] = children[i]->box.Intersects(mesh->GetAABB());
                if (intersects[i])
                {
                    intersection_count += 1;
                }
            }

            // If it intersects more than desired dont go downwards
            if (intersection_count > 1)
            {
                ++it;
                continue;
            }

            for (unsigned i = 0; i < static_cast<unsigned>(Quadrants::COUNT); ++i)
            {
                if (intersects[i])
                {
                    children[i]->meshes.push_back(mesh);
                }
            }

            it = decltype(it)(meshes.erase((it + 1).base()));
        }
    }

    unsigned children_meshes = 0;
    for (unsigned i = 0; i < static_cast<unsigned>(Quadrants::COUNT); ++i)
    {
       children_meshes += children[i]->RearangeChildren();
    }

    // Reduce the tree size when possible
    if ((children_meshes + meshes.size()) < QUADTREE_MAX_ITEMS)
    {
        for (unsigned i = 0; i < static_cast<unsigned>(Quadrants::COUNT); ++i)
        {
            QuadtreeNode* child = children[i];
            std::vector<ComponentMeshRenderer*>& child_meshes = child->meshes;
            // No need to clear child meshes since it will be destroyed
            meshes.insert(meshes.end(), child_meshes.begin(), child_meshes.end());
        }
        DeleteChildren();
        return meshes.size();
    }

    return children_meshes + meshes.size();
}


void Hachiko::QuadtreeNode::GetIntersections(std::vector<ComponentMeshRenderer*>& intersected, const Frustum& frustum)
{
    if (frustum.Intersects(box))
    {
        for (ComponentMeshRenderer* mesh : meshes)
        {
            if (frustum.Intersects(mesh->GetAABB()))
            {
                intersected.push_back(mesh);
            }
        }

        // If it has one child all exist
        if (children[0] != nullptr)
        {
            for (int i = 0; i < 4; ++i)
            {
                children[i]->GetIntersections(intersected, frustum);
            }
        }
    }
}

void Hachiko::QuadtreeNode::DebugDraw()
{
    static const int order[8] = {0, 1, 5, 4, 2, 3, 7, 6};
    if (IsLeaf())
    {
        ddVec3 p[8];

        for (int i = 0; i < 8; ++i)
        {
            p[i] = box.CornerPoint(order[i]);
        }

        dd::box(p, dd::colors::LawnGreen);
        return;
    }

    for (QuadtreeNode* child : children)
    {
        child->DebugDraw();
    }
}

Hachiko::Quadtree::Quadtree() = default;

Hachiko::Quadtree::~Quadtree()
{
    Clear();
}

void Hachiko::Quadtree::Clear()
{
    RELEASE(root);
}

void Hachiko::Quadtree::SetBox(const AABB& box)
{
    Clear();
    root = new QuadtreeNode(box, nullptr, 0);
}

void Hachiko::Quadtree::Reposition(ComponentMeshRenderer* mesh)
{
    if (root)
    {
        to_remove.insert(mesh);
        to_insert.insert(mesh);
    }
}

void Hachiko::Quadtree::Remove(ComponentMeshRenderer* mesh)
{
    // Removes the element from insert commands to make sure it doesnt stay on the quadtree if reposiiton was called
    if (to_insert.find(mesh) != to_insert.end())
    {
        to_insert.erase(mesh);
    }
    to_remove.insert(mesh);
}


void Hachiko::Quadtree::Refresh()
{
    // This method assumes root exists
    bool dirty = false;

    if (!to_remove.empty())
    {
        root->Remove(to_remove);
        to_remove.clear();
        dirty = true;
    }
    if (!to_insert.empty())
    {
        root->Insert(to_insert);
        to_insert.clear();
        dirty = true;
    }

    if (dirty)
    {
        root->RearangeChildren();
    }
}
void Hachiko::Quadtree::DebugDraw()
{
    if (root)
    {
        Refresh();
        root->DebugDraw();
    }
}