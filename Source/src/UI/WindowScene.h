#pragma once
#include "Window.h"
#include <imgui.h>
#include <ImGuizmo.h>

#include "../Utils/Timer.h"
#include "MathGeoLib.h"

class GameObject;

class WindowScene : public Window
{
public:
	WindowScene();
	void Update() override;

	bool ToolbarButton(ImFont* font, const char* font_icon, bool active);

private:
	void GuizmoOptionsController();
	void ToolbarMenu();
	void DrawScene();
	void Controller();
	GameObject* SelectObject();

	ImVec2 imguizmo_size = {100.0f, 100.0f};
	bool using_guizmo = false;
	
	Timer* scene_timer = nullptr;

	ImGuizmo::OPERATION guizmo_operation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE guizmo_mode = ImGuizmo::WORLD;

	ImVec2 guizmo_rect_origin = {0.0f, 0.0f};
	float2 texture_size = {0.0f, 0.0f};
};
