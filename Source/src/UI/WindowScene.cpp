#include "WindowScene.h"

#include "../Application.h"
#include "../Modules/ModuleInput.h"
#include "../Modules/ModuleEditor.h"
#include "../Modules/ModuleCamera.h"

#include "../Components/ComponentCamera.h"

#include <IconsFontAwesome5.h>
#include <imgui_internal.h>

WindowScene::WindowScene()
	: Window("Scene", true)
{
	scene_timer = new Timer();
}

void WindowScene::Update()
{
	if (!App->input->GetMouseButton(SDL_BUTTON_RIGHT))
	{
		if (App->input->GetKey(SDL_SCANCODE_W)) current_guizmo_operation = ImGuizmo::TRANSLATE;
		if (App->input->GetKey(SDL_SCANCODE_E)) current_guizmo_operation = ImGuizmo::ROTATE;
		if (App->input->GetKey(SDL_SCANCODE_R)) current_guizmo_operation = ImGuizmo::SCALE;
	}
	if (ImGui::Begin(ICON_FA_GLOBE "Scene", &active))
	{
		GuizmoControl();

		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();

		SceneControl();
		DrawScene();
	};
}

void WindowScene::DrawScene()
{
	ImVec2 size = ImGui::GetContentRegionAvail();

	App->camera->SetAspectRatio((unsigned) size.x, (unsigned) size.y);
	App->renderer->WindowResized(size.x, size.y);

	ImGui::Image((ImTextureID)(App->camera->getMainCamera()->GetTextureId()), size, ImVec2 {0, 1}, ImVec2 {1, 0});

	ImGui::End();

	// TO AVOID Camera orbit
	if (ImGui::IsWindowFocused())
	{
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) || App->input->GetKey(SDL_SCANCODE_LALT))
		{
			ImGuizmo::Enable(false);
		}
		else
		{
			ImGuizmo::Enable(true);
		}
	}

	GameObject* selected_object = App->editor->getSelectedGO();

	if (selected_object) {
		// Get transform and try to decompose or something like
	}

}

void WindowScene::SceneControl()
{
	float w = (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) * 0.5f - 30 - ImGui::GetCursorPosX();
	ImGui::Dummy(ImVec2(w, ImGui::GetTextLineHeight()));

	if (scene_timer->HasGameStarted())
	{
		if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_STOP)) scene_timer->StopGame();

		if (scene_timer->IsGameRunning())
		{
			if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_PAUSE)) scene_timer->PauseGame();
		}
		else
		{
			if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_PLAY)) scene_timer->ResumeGame();
		}
	}
	else
	{
		if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_PLAY)) scene_timer->StartGame();
	}

	if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_STEP_FORWARD)) scene_timer->StepGame();
	ImGui::Separator();
}

void WindowScene::GuizmoControl()
{
	if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_ARROWS_ALT)) current_guizmo_operation = ImGuizmo::TRANSLATE; 
	if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_UNDO)) current_guizmo_operation = ImGuizmo::ROTATE; 
	if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_EXPAND_ALT))  current_guizmo_operation = ImGuizmo::SCALE; 

	ImGui::SameLine();
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
	ImGui::SameLine();

	if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_HOME)) current_guizmo_mode = ImGuizmo::LOCAL;
	if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_GLOBE)) current_guizmo_mode = ImGuizmo::WORLD;
}

//TODO: Move to utils gui and add tooltips
bool WindowScene::ToolbarButton(ImFont* font, const char* font_icon)
{
	const ImVec4 col_active = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
	const ImVec4 bg_color = false ? col_active : ImGui::GetStyle().Colors[ImGuiCol_Text];

	ImGui::SameLine();
	auto frame_padding = ImGui::GetStyle().FramePadding;
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_Text, bg_color);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, ImGui::GetStyle().FramePadding.y));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, frame_padding);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

	ImGui::PushFont(font);
	if (ImGui::Button(font_icon))
	{
		return true;
	}

	ImGui::PopFont();
	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar(3);

	return false;
}
