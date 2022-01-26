#pragma once

#include "Globals.h"
#include "MathGeoLib.h"

#include "GameObject.h"

#include <list>
#include <map>

#define QUADTREE_MAX_ITEMS 8
#define QUADTREE_MIN_SIZE 10.0f

enum Quadrants
{
	NW = 0,
	NE,
	SE,
	SW,
	NUM_QUADRANTS
};

class QuadtreeNode
{
public:
	QuadtreeNode(const AABB& box, QuadtreeNode* parent);
	~QuadtreeNode();

	void Insert(GameObject* game_object);
	void Remove(GameObject* game_object);
	void CreateChildren();
	void RearangeChildren();

	bool IsLeaf() const { return childs[0] == nullptr; }

	const AABB& GetBox() const { return box; }
	const std::list<GameObject*>& GetObjects() const { return objects; }

	template<typename T>
	void GetIntersections(std::map<float, GameObject*>& objects, const T& primitive) const;

	QuadtreeNode* childs[Quadrants::NUM_QUADRANTS];

	void DebugDraw();

private:
	AABB box;
	QuadtreeNode* parent;	
	std::list<GameObject*> objects;
	
};

class Quadtree
{
public:
	Quadtree();
	~Quadtree();

	void Clear();
	void SetBox(const AABB& box);

	void Insert(GameObject* game_object);
	void Remove(GameObject* game_object);

	inline QuadtreeNode* GetRoot() { return root; }

	void DebugDraw();
		
private:
	QuadtreeNode* root = nullptr;
};


template<typename T>
void QuadtreeNode::GetIntersections(std::map<float, GameObject*>& intersected, const T& primitive) const
{
	if (primitive.Intersects(box))
	{
		float near_hit, far_hit;
		for (GameObject* object : objects)
		{
			if (primitive.Intersects(object->GetOBB()))
				intersected[near_hit] = object;
		}

		// If it has one child all exist
		if (childs[0] != nullptr) {
			for (int i = 0; i < 4; ++i)
				childs[i]->GetIntersections(intersected, primitive);
		}
	}
}