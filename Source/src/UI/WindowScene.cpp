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
}

void WindowScene::Update()
{
	if (ImGui::Begin(ICON_FA_GLOBE "Scene", &active)) {
		float z = (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) * 0.15f - 30 - ImGui::GetCursorPosX();
		ImGui::Dummy(ImVec2(z, ImGui::GetTextLineHeight()));

		ToolbarButton(App->editor->m_big_icon_font, ICON_FA_UNDO);
		ToolbarButton(App->editor->m_big_icon_font, ICON_FA_EXPAND_ALT);
		ToolbarButton(App->editor->m_big_icon_font, ICON_FA_HOME);
		ToolbarButton(App->editor->m_big_icon_font, ICON_FA_GLOBE);
		
		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();

		float w = (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) * 0.5f - 30 - ImGui::GetCursorPosX();
		ImGui::Dummy(ImVec2(w, ImGui::GetTextLineHeight()));

		ToolbarButton(App->editor->m_big_icon_font, ICON_FA_PLAY);
		ToolbarButton(App->editor->m_big_icon_font, ICON_FA_PAUSE);
		ToolbarButton(App->editor->m_big_icon_font, ICON_FA_STEP_FORWARD);
		ImGui::Separator();

		ImVec2 size = ImGui::GetContentRegionAvail();
		App->renderer->WindowResized(size.x, size.y);
		App->camera->SetAspectRatio((unsigned) size.x, (unsigned) size.y);

		ImGui::Image((ImTextureID)(App->camera->getMainCamera()->GetTextureId()), size, ImVec2 {0, 1}, ImVec2 {1, 0});

		ImGui::End();
	};
}

//TODO: Move to utils gui
void WindowScene::ToolbarButton(ImFont* font, const char* font_icon)
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
		// TODO: function
	}

	ImGui::PopFont();
	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar(3);
}
