#include "WindowHierarchy.h"
#include <imgui.h>
#include <IconsFontAwesome5.h>

#include "../Modules/ModuleSceneManager.h"
#include "../Modules/ModuleInput.h"
#include "../Modules/ModuleEditor.h"

#include "../Application.h"
#include "../GameObject.h"
#include "../Scene.h"


WindowHierarchy::WindowHierarchy()
	: Window("Hierarchy", true)
{
}

void WindowHierarchy::Update()
{
	if (ImGui::Begin(ICON_FA_SITEMAP "Hierarchy", &active))
	{
		DrawHierarchyTree(App->scene_manager->GetRoot());
	}
	ImGui::End();
}


void WindowHierarchy::DrawHierarchyTree(GameObject* root)
{
	DrawGOChilds(root);
}

void WindowHierarchy::DrawGOChilds(GameObject* root)
{
	for (GameObject* game_object : root->childs)
	{
		DrawGameObject(game_object);
	}
}

void WindowHierarchy::DrawGameObject(GameObject* game_object)
{
	ShowGameObject(game_object);

	if (ImGui::IsItemHovered())
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT)) {
			App->editor->setSelectedGO(game_object);
		}
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT))
		{
			ImGui::OpenPopup(game_object->name.c_str());
		}
	}

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
			// TODO: Make it work
			//game_object->Destroy();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::MenuItem("Move Up"))
		{
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::MenuItem("Move Down"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (game_object->hierarchy_open == true)
	{
		DrawGOChilds(game_object);
		ImGui::TreePop();
	}
}

void WindowHierarchy::ShowGameObject(GameObject* game_object)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (game_object->childs.empty())
	{
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	bool nodeOpen = ImGui::TreeNodeEx(game_object, flags, game_object->name.c_str());
	game_object->hierarchy_open = game_object->childs.empty() ? false : nodeOpen;
}