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

private:
	AABB box;
	QuadtreeNode* parent;
	QuadtreeNode* childs[Quadrants::NUM_QUADRANTS];
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
		

private:
	QuadtreeNode* root = nullptr;
};
