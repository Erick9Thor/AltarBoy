#include "core/hepch.h"
#include "WindowHierarchy.h"

#include "modules/ModuleSceneManager.h"
#include "modules/ModuleInput.h"
#include "modules/ModuleEditor.h"

Hachiko::WindowHierarchy::WindowHierarchy() :
    Window("Hierarchy", true) {}

Hachiko::WindowHierarchy::~WindowHierarchy() = default;

void Hachiko::WindowHierarchy::Update()
{
    ImGui::SetNextWindowDockID(App->editor->dock_left_id, ImGuiCond_FirstUseEver);
    if (ImGui::Begin((std::string(ICON_FA_SITEMAP " ") + name).c_str(), &active))
    {
        DrawHierarchyTree(App->scene_manager->GetRoot());
    }
    ImGui::End();
}

void Hachiko::WindowHierarchy::CleanUp()
{
    dragged_object = nullptr;
    delete dragged_object;
}

void Hachiko::WindowHierarchy::DrawHierarchyTree(const GameObject* game_object)
{
    DrawChildren(game_object);
    if (!App->input->GetMouseButton(SDL_BUTTON_LEFT))
    {
        dragged_object = nullptr;
    }
}

void Hachiko::WindowHierarchy::DrawChildren(const GameObject* game_object)
{
    for (const auto go : game_object->children)
    {
        ImGui::PushID(go);
        DrawGameObject(go);
        ImGui::PopID();
    }
}

void Hachiko::WindowHierarchy::DrawGameObject(GameObject* game_object)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

    if (game_object->children.empty())
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (game_object == App->editor->GetSelectedGameObject())
    {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    ImVec4 node_color = ImGui::GetStyle().Colors[ImGuiCol_Text];
    if (!game_object->IsActive())
    {
        node_color = ImVec4(1.f, 0.f, 0.f, 1.f);
    }

    ImGui::PushStyleColor(ImGuiCol_Text, node_color);

    const bool node_open = ImGui::TreeNodeEx(game_object, flags, game_object->name.c_str());

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

    if (node_open && !game_object->children.empty())
    {
        DrawChildren(game_object);
    }

    // TODO: Make robust to repeted game object names
    if (ImGui::BeginPopup(game_object->name.c_str()))
    {
        // Alternativs: ImGui::Selectable, ImGuiHelper::ValueSelection
        // TODO: Open options to create/destroy new object or move up down in the list of children
        if (ImGui::MenuItem("Add Game Object"))
        {
            GameObject* selectedGameObject = App->editor->GetSelectedGameObject();
            if (selectedGameObject == game_object)
            {
                App->editor->SetSelectedGO(nullptr);
            }

            App->scene_manager->GetActiveScene()->CreateNewGameObject("New Game Object", game_object);
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("Delete Game Object"))
        {
            RELEASE(game_object);
            dragged_object = nullptr;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (node_open)
    {
        ImGui::TreePop();
    }

    ImGui::PopStyleColor();
}
