#pragma once
#include "Window.h"

class GameObject;

class WindowInspector : public Window
{
public:
	WindowInspector();
	void Update() override;

private:
	void DrawGameObject(GameObject* go);
};
