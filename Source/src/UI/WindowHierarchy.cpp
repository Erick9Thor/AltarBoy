#include "WindowHierarchy.h"
#include <imgui.h>
#include <IconsFontAwesome5.h>

#include "../Modules/ModuleSceneManager.h"
#include "../Modules/ModuleInput.h"
#include "../Modules/ModuleEditor.h"

#include "../Application.h"
#include "../GameObject.h"
#include "../Scene.h"

#include "../Utils/Logger.h"

WindowHierarchy::WindowHierarchy()
	: Window("Hierarchy", true)
{
}

WindowHierarchy::~WindowHierarchy()
{
}

void WindowHierarchy::Update()
{
	ImGui::SetNextWindowDockID(App->editor->dock_left_id, ImGuiCond_FirstUseEver);
	if (ImGui::Begin((std::string(ICON_FA_SITEMAP " ") + name).c_str(), &active))
	{
		DrawHierarchyTree(App->scene_manager->GetRoot());
	}
	ImGui::End();
}

void WindowHierarchy::CleanUp()
{
	dragged_object = nullptr;
	delete dragged_object;
}

void WindowHierarchy::DrawHierarchyTree(GameObject* root)
{
	DrawGOChilds(root);
	if (!App->input->GetMouseButton(SDL_BUTTON_LEFT))
		dragged_object = nullptr;
}

void WindowHierarchy::DrawGOChilds(GameObject* root)
{
	for (GameObject* game_object : root->childs)
	{
		ImGui::PushID(game_object);
		DrawGameObject(game_object);
		ImGui::PopID();
	}
}

void WindowHierarchy::DrawGameObject(GameObject* game_object)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_DefaultOpen;

	if (game_object->childs.empty())
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (game_object == App->editor->GetSelectedGO())
		flags |= ImGuiTreeNodeFlags_Selected;

	ImVec4 node_color(1.0f, 1.0f, 1.0f, 1.0f);
	if (!game_object->IsActive())
		node_color = ImVec4(1.f, 0.f, 0.f, 1.f);

	ImGui::PushStyleColor(ImGuiCol_Text, node_color);

	bool node_open = ImGui::TreeNodeEx(game_object, flags, game_object->name.c_str());

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly))
	{
		if (dragged_object && dragged_object != game_object)
		{
			ImGui::BeginTooltip();
			ImGui::Text("% s->% s ", dragged_object->name.c_str(), game_object->name.c_str());
			ImGui::EndTooltip();
		}

		if (ImGui::IsMouseClicked(0))
		{
			App->editor->SetSelectedGO(game_object);
			dragged_object = game_object;
		}
		if (ImGui::IsMouseClicked(1))
		{
			ImGui::OpenPopup(game_object->name.c_str());
		}

		if (dragged_object && ImGui::IsMouseReleased(0) && dragged_object != game_object)
		{
			dragged_object->SetNewParent(game_object);
			dragged_object = nullptr;
		}
	}

	if (node_open && !game_object->childs.empty())
		DrawGOChilds(game_object);

	// TODO: Make robust to repeted game object names
	if (ImGui::BeginPopup(game_object->name.c_str()))
	{
		// Alternativs: ImGui::Selectable, ImGuiHelper::ValueSelection
		// TODO: Open options to create/destroy new object or move up down in the list of childs
		if (ImGui::MenuItem("Add Gameojbect"))
		{
			App->scene_manager->GetActiveScene()->CreateNewGameObject("Test", game_object);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::MenuItem("Delete Gameojbect"))
		{
			GameObject* selectedGameObject = App->editor->GetSelectedGO();
			if (selectedGameObject == game_object)
			{
				App->editor->SetSelectedGO(nullptr);
			}

			RELEASE(game_object);
			dragged_object = nullptr;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (node_open)
		ImGui::TreePop();

	ImGui::PopStyleColor();
}