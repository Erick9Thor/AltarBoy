#include "core/hepch.h"
#include "Quadtree.h"
#include <debugdraw.h>

Hachiko::QuadtreeNode::QuadtreeNode(const AABB& box, QuadtreeNode* parent) :
    box(box),
    parent(parent)
{
    children[static_cast<int>(Quadrants::NW)] = children[static_cast<int>(Quadrants::NE)] = children[static_cast<int>(Quadrants::SE)] = children[static_cast<int>(Quadrants::SW)] = nullptr;
}

Hachiko::QuadtreeNode::~QuadtreeNode()
{
    RELEASE(children[(int)Quadrants::NW]);
    RELEASE(children[(int)Quadrants::NE]);
    RELEASE(children[(int)Quadrants::SE]);
    RELEASE(children[(int)Quadrants::SW]);
}

void Hachiko::QuadtreeNode::Insert(GameObject* game_object)
{
    // No split due to not enough objects
    if (IsLeaf() && objects.size() < QUADTREE_MAX_ITEMS)
    {
        objects.push_back(game_object);
        return;
    }
    // No split due to minimum size
    if (box.HalfSize().LengthSq() <= QUADTREE_MIN_SIZE * QUADTREE_MIN_SIZE)
    {
        objects.push_back(game_object);
        return;
    }
    // Rearrange children
    if (IsLeaf())
    {
        CreateChildren();
    }

    objects.push_back(game_object);
    RearangeChildren();
}

void Hachiko::QuadtreeNode::Remove(GameObject* game_object)
{
    // TODO: Reduce size when no longer necessary?
    const auto find_iter = std::find(objects.begin(), objects.end(), game_object);
    if (find_iter != objects.end())
    {
        objects.erase(find_iter);
    }

    if (!IsLeaf())
    {
        for (const auto& child : children)
        {
            child->Remove(game_object);
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

    // NW
    child_center.x = center.x - size.x * 0.25f;
    child_center.z = center.z + size.z * 0.25f;
    child_box.SetFromCenterAndSize(child_center, child_size);
    children[static_cast<int>(Quadrants::NW)] = new QuadtreeNode(child_box, this);

    // NE
    child_center.x = center.x + size.x * 0.25f;
    child_center.z = center.z + size.z * 0.25f;
    child_box.SetFromCenterAndSize(child_center, child_size);
    children[static_cast<int>(Quadrants::NE)] = new QuadtreeNode(child_box, this);

    // SE
    child_center.x = center.x + size.x * 0.25f;
    child_center.z = center.z - size.z * 0.25f;
    child_box.SetFromCenterAndSize(child_center, child_size);
    children[static_cast<int>(Quadrants::SE)] = new QuadtreeNode(child_box, this);

    // SW
    child_center.x = center.x - size.x * 0.25f;
    child_center.z = center.z - size.z * 0.25f;
    child_box.SetFromCenterAndSize(child_center, child_size);
    children[static_cast<int>(Quadrants::SW)] = new QuadtreeNode(child_box, this);
}

void Hachiko::QuadtreeNode::RearangeChildren()
{
    for (auto it = objects.begin(); it != objects.end();)
    {
        GameObject* game_object = *it;
        bool intersects[static_cast<int>(Quadrants::COUNT)];
        for (int i = 0; i < static_cast<int>(Quadrants::COUNT); ++i)
        {
            intersects[i] = children[i]->box.Intersects(game_object->GetAABB());
        }

        // If it intersects in more than one quadrant dont move it downwards
        int n_intersects = intersects[static_cast<int>(Quadrants::NW)] + intersects[static_cast<int>(Quadrants::NE)] + intersects[static_cast<int>(Quadrants::SE)] + intersects[static_cast<int>(Quadrants::SW)];
        if (n_intersects > 1)
        {
            ++it;
            continue;
        }

        for (int i = 0; i < static_cast<int>(Quadrants::COUNT); ++i)
        {
            if (intersects[i])
            {
                children[i]->Insert(game_object);
            }
        }
        it = objects.erase(it);
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
    root = new QuadtreeNode(box, nullptr);
}

void Hachiko::Quadtree::Insert(GameObject* game_object) const
{
    if (root)
    {
        root->Insert(game_object);
    }
}

void Hachiko::Quadtree::Remove(GameObject* game_object) const
{
    if (root)
    {
        root->Remove(game_object);
    }
}

void Hachiko::Quadtree::DebugDraw() const
{
    if (root)
    {
        root->DebugDraw();
    }
}
