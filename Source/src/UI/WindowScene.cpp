#include "WindowScene.h"

#include "../Application.h"
#include "../GameObject.h"

#include "../Modules/ModuleInput.h"
#include "../Modules/ModuleEditor.h"
#include "../Modules/ModuleCamera.h"

#include "../Components/ComponentCamera.h"
#include "../Components/ComponentTransform.h"

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
	gizmo_rect_origin = {
		size.x,
		size.y,
	};

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImDrawVert* last_vertex = draw_list->VtxBuffer.end();
	texture_size = {
		last_vertex->pos.x,
		last_vertex->pos.y,
	};

	ImGui::Image((ImTextureID)(App->camera->getMainCamera()->GetTextureId()), size, ImVec2 {0, 1}, ImVec2 {1, 0});


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

	//TODO: Fix 

	float view_manipulate_right = gizmo_rect_origin.x + texture_size.x;
	float view_manipulate_top = gizmo_rect_origin.y;
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(gizmo_rect_origin.x, gizmo_rect_origin.y, texture_size.x, texture_size.y);

	GameObject* selected_object = App->editor->getSelectedGO();

	float4x4 m_view = App->camera->getMainCamera()->GetViewMatrix(true);
	float4x4 m_projection = App->camera->getMainCamera()->GetProjectionMatrix(true);
	if (selected_object) {
		float4x4 model_projection = selected_object->GetComponent<ComponentTransform>()->GetTransform().Transposed();

		ImGuizmo::Manipulate(m_view.ptr(), m_projection.ptr(), current_guizmo_operation, current_guizmo_mode, model_projection.ptr(), nullptr, nullptr);
		if (ImGuizmo::IsUsing())
		{
			float4x4 newMatrix;
			newMatrix.Set(model_projection);
			selected_object->GetComponent<ComponentTransform>()->SetGlobalTransform(newMatrix.Transposed());
		}
	}

	ImGuizmo::ViewManipulate(m_view.ptr(), 4, ImVec2(view_manipulate_right - imguizmo_size, view_manipulate_top), ImVec2(imguizmo_size, imguizmo_size), 0x10101010);
	
	//TODO ADD look at when manipulating cube

	ImGui::End();
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
