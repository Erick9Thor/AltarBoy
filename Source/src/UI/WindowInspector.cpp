#include "WindowInspector.h"

#include "../Application.h"
#include "../GameObject.h"

#include "../Modules/ModuleEditor.h"

#include "imgui.h"
#include <IconsFontAwesome5.h>

WindowInspector::WindowInspector()
	: Window("Inspector", true) {}

void WindowInspector::Update()
{
	if (ImGui::Begin(ICON_FA_EYE "Inspector", &active))
	{
		InspectorDrawGameObject(App->editor->GetSelectedGO());
	}
	ImGui::End();
}

void WindowInspector::InspectorDrawGameObject(GameObject* game_object)
{
	if (game_object != nullptr)
	{
		char go_name[50];
		strcpy_s(go_name, 50, game_object->name.c_str());
		ImGuiInputTextFlags name_input_flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
		if (ImGui::InputText("###", go_name, 50, name_input_flags))
			game_object->name = go_name;

		std::vector<Component*> go_components = game_object->GetComponents();
		for (vector<Component*>::iterator it = go_components.begin(); it != go_components.end(); ++it)
			(*it)->DrawGui();

		ImGui::Separator();
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("Add Component Popup");
		}

		if (ImGui::BeginPopup("Add Component Popup"))
		{
			if (ImGui::MenuItem("Mesh"))
			{
				game_object->CreateComponent(Component::Type::Mesh);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Material"))
			{
				game_object->CreateComponent(Component::Type::Material);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Camera"))
			{
				game_object->CreateComponent(Component::Type::Camera);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}
