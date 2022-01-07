#pragma once
#include "Globals.h"
#include "MathGeoLib.h"

#include <list>

//#define NE 0

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

class GameObject;

class QuadtreeNode
{
public:
	QuadtreeNode(const AABB& box, QuadtreeNode* parent);
	~QuadtreeNode();

	void Insert(GameObject* game_object);
	void Remove(GameObject* game_object);
	void CreateChildren();
	void RearangeChildren();

	bool IsLeaf() const;

	const AABB& GetBox() const
	{
		return box;
	
	}

	const std::list<GameObject*>& GetObjects() const {
		return objects;
	}

	QuadtreeNode* GetChildren() const {
		return childs[0];
	}

	QuadtreeNode* childs[Quadrants::NUM_QUADRANTS];

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

	inline QuadtreeNode* GetRoot()
	{
		return root;
	}
		

private:
	QuadtreeNode* root = nullptr;
};
