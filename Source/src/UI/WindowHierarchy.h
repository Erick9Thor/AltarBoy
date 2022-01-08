#pragma once
#include "Window.h"

class GameObject;

class WindowHierarchy : public Window
{
public:
	WindowHierarchy();
	void Update() override;

private:
	void DrawHierarchyTree(GameObject* root);
	void DrawGOChilds(GameObject* root);
	void DrawGameObject(GameObject* go);
	void ShowGameObject(GameObject* go);

	GameObject* dragged_object;
};
