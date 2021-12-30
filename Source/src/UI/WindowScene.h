#pragma once
#include "Window.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include "../Utils/Timer.h"

class WindowScene : public Window
{
public:
	WindowScene();
	void Update() override;

	bool ToolbarButton(ImFont* font, const char* font_icon);

private:
	void GuizmoControl();
	void SceneControl();
	void DrawScene();
	
	Timer* scene_timer = nullptr;

	ImGuizmo::OPERATION current_guizmo_operation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE current_guizmo_mode = ImGuizmo::WORLD;
};
