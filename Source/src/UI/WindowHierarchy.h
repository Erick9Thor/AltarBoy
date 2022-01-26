#pragma once
#include "Window.h"

class GameObject;

class WindowHierarchy : public Window
{
public:
	WindowHierarchy();
	~WindowHierarchy() override;
	void Update() override;

	void CleanUp() override;

private:
	void DrawHierarchyTree(GameObject* root);
	void DrawGOChilds(GameObject* root);
	void DrawGameObject(GameObject* go);

	GameObject* dragged_object = nullptr;
};
