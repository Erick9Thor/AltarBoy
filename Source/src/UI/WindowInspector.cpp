#include "WindowInspector.h"

#include "Application.h"
#include "Core/GameObject.h"

#include "Modules/ModuleEditor.h"

#include "imgui.h"
#include <IconsFontAwesome5.h>

WindowInspector::WindowInspector()
	: Window("Inspector", true) {}

WindowInspector::~WindowInspector()
{
}

void WindowInspector::Update()
{
	ImGui::SetNextWindowDockID(App->editor->dock_right_id, ImGuiCond_FirstUseEver);
	if (ImGui::Begin((std::string(ICON_FA_EYE " ") + name).c_str(), &active))
	{
		DrawGameObject(App->editor->GetSelectedGO());
	}
	ImGui::End();
}

void WindowInspector::DrawGameObject(GameObject* game_object)
{
	if (game_object != nullptr)
	{
		char go_name[50];
		strcpy_s(go_name, 50, game_object->name.c_str());
		ImGuiInputTextFlags name_input_flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
		if (ImGui::InputText("###", go_name, 50, name_input_flags))
			game_object->name = go_name;

		ImGui::SameLine();
		ImGui::Checkbox("Active", &game_object->active);

		std::vector<Component*> go_components = game_object->GetComponents();
		for (vector<Component*>::iterator it = go_components.begin(); it != go_components.end(); ++it)
			(*it)->DrawGui();

		ImGui::Separator();
		const float x = (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize((std::string(ICON_FA_PLUS " ") + "Add component").c_str()).x - ImGui::GetStyle().FramePadding.x * 2) * 0.5f;
		ImGui::SetCursorPosX(x);
		if (ImGui::Button((std::string(ICON_FA_PLUS " ") + "Add component").c_str()))
		{
			ImGui::OpenPopup("AddComponentPopup");
		}

		if (ImGui::BeginPopup("AddComponentPopup"))
		{
			
			if (ImGui::MenuItem("Camera"))
			{
				game_object->CreateComponent(Component::Type::CAMERA);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Point Light"))
			{
				game_object->CreateComponent(Component::Type::POINTLIGHT);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Spot Light"))
			{
				game_object->CreateComponent(Component::Type::SPOTLIGHT);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Dir Light"))
			{
				game_object->CreateComponent(Component::Type::DIRLIGHT);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}
