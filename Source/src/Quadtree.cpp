#include "Quadtree.h"

#include "GameObject.h"

QuadtreeNode::QuadtreeNode(const AABB& box, QuadtreeNode* parent)
	: box(box)
	, parent(parent)
{
	childs[Quadrants::NW] = childs[Quadrants::NE] = childs[Quadrants::SE] = childs[Quadrants::SW] = nullptr;
}

QuadtreeNode::~QuadtreeNode()
{
	RELEASE(childs[Quadrants::NW]);
	RELEASE(childs[Quadrants::NE]);
	RELEASE(childs[Quadrants::SE]);
	RELEASE(childs[Quadrants::SW]);
}

void QuadtreeNode::Insert(GameObject* game_object)
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
	// Rearrange childs
	if (IsLeaf())
		CreateChildren();
	
	objects.push_back(game_object);
	RearangeChildren();

}

void QuadtreeNode::Remove(GameObject* game_object)
{
	// TODO: Reduce size when no longer necessary?
	std::list<GameObject*>::iterator find_iter = std::find(objects.begin(), objects.end(), game_object);
	if (find_iter != objects.end())
		objects.erase(find_iter);

	if (!IsLeaf()) {
		for (int i = 0; i < Quadrants::NUM_QUADRANTS; ++i)
			childs[i]->Remove(game_object);
	}
}

void QuadtreeNode::CreateChildren()
{
	// Subdivide current box
	float3 size = float3(box.Size());
	float3 center = box.CenterPoint();
	// Do not operate y, 2d
	float3 child_size = float3(0.5f * size.x, size.y, 0.5f * size.z);
	// Recalculate for each child
	float3 child_center;
	AABB child_box;

	// NW
	child_center.x = center.x - size.x * 0.25f;
	child_center.x = center.z + size.z * 0.25f;
	child_box.SetFromCenterAndSize(child_center, child_size);
	childs[Quadrants::NW] = new QuadtreeNode(child_box, this);

	// NE
	child_center.x = center.x + size.x * 0.25f;
	child_center.x = center.z + size.z * 0.25f;
	child_box.SetFromCenterAndSize(child_center, child_size);
	childs[Quadrants::NE] = new QuadtreeNode(child_box, this);

	// SE
	child_center.x = center.x + size.x * 0.25f;
	child_center.x = center.z - size.z * 0.25f;
	child_box.SetFromCenterAndSize(child_center, child_size);
	childs[Quadrants::SE] = new QuadtreeNode(child_box, this);

	// SW
	child_center.x = center.x - size.x * 0.25f;
	child_center.x = center.z - size.z * 0.25f;
	child_box.SetFromCenterAndSize(child_center, child_size);
	childs[Quadrants::SW] = new QuadtreeNode(child_box, this);
}

void QuadtreeNode::RearangeChildren()
{
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end();)
	{
		GameObject* game_object = *it;
		bool intersects[Quadrants::NUM_QUADRANTS];
		for (int i = 0; i < Quadrants::NUM_QUADRANTS; ++i)
			intersects[i] = childs[i]->box.Intersects(game_object->GetAABB());

		// If it intersects all there is no point in moving downwards
		if (intersects[Quadrants::NW] && intersects[Quadrants::NE] && intersects[Quadrants::SE] && intersects[Quadrants::SW])
			++it;
		else {
			for (int i = 0; i < Quadrants::NUM_QUADRANTS; ++i) {
				if (intersects[i])
					childs[i]->Insert(game_object);
			}				
			it = objects.erase(it);
		}			
	}
}

bool QuadtreeNode::IsLeaf() const
{
	return childs[0] == nullptr;
}

Quadtree::Quadtree()
{
}

Quadtree::~Quadtree()
{
	Clear();
}

void Quadtree::Clear()
{
	RELEASE(root);
}

void Quadtree::SetBox(const AABB& box)
{
	Clear();
	root = new QuadtreeNode(box, nullptr);
}

void Quadtree::Insert(GameObject* game_object)
{
	if (root) root->Insert(game_object);
}

void Quadtree::Remove(GameObject* game_object)
{
	if (root) root->Remove(game_object);
}
