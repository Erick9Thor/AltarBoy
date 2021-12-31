#pragma once
#include "Window.h"
#include <imgui.h>
#include <ImGuizmo.h>

#include "../Utils/Timer.h"
#include "../Utils/Math.h"

class WindowScene : public Window
{
public:
	WindowScene();
	void Update() override;

	bool ToolbarButton(ImFont* font, const char* font_icon, bool active);

private:
	void GuizmoControl();
	void SceneControl();
	void DrawScene();

	float imguizmo_size = 100;
	
	Timer* scene_timer = nullptr;

	ImGuizmo::OPERATION current_guizmo_operation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE current_guizmo_mode = ImGuizmo::WORLD;

	float2 gizmo_rect_origin = {0.0f, 0.0f};
	float2 texture_size = {0.0f, 0.0f};
};
