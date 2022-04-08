#include "core/hepch.h"
#include "WindowInspector.h"

#include "modules/ModuleEditor.h"

Hachiko::WindowInspector::WindowInspector() :
    Window("Inspector", true) {}

Hachiko::WindowInspector::~WindowInspector() = default;

void Hachiko::WindowInspector::Update()
{
    ImGui::SetNextWindowDockID(App->editor->dock_right_id, ImGuiCond_FirstUseEver);
    if (ImGui::Begin((std::string(ICON_FA_EYE " ") + name).c_str(), &active))
    {
        DrawGameObject(App->editor->GetSelectedGameObject());
    }
    ImGui::End();
}

void Hachiko::WindowInspector::DrawGameObject(GameObject* game_object) const
{
    if (game_object == nullptr)
    {
        return;
    }
    char go_name[50];
    strcpy_s(go_name, 50, game_object->GetName().c_str());
    const ImGuiInputTextFlags name_input_flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
    if (ImGui::InputText("###", go_name, 50, name_input_flags))
    {
        game_object->SetName(go_name);
    }

    ImGui::SameLine();
    bool activate = game_object->IsActive();
    if (ImGui::Checkbox("Active", &activate))
    {
        activate ? game_object->Enable() : game_object->Disable();
    }

    std::vector<Component*> go_components = game_object->GetComponents();
    for (auto it = go_components.begin(); it != go_components.end(); ++it)
    {
        (*it)->DrawGui();
    }

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
        if (ImGui::MenuItem("Mesh"))
        {
            game_object->CreateComponent(Component::Type::MESH);
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("Material"))
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

            if (ImGui::MenuItem("Canvas"))
            {
                game_object->CreateComponent(Component::Type::TRANSFORM_2D);
                game_object->CreateComponent(Component::Type::CANVAS);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Button"))
            {                
                game_object->CreateComponent(Component::Type::TRANSFORM_2D);
                game_object->CreateComponent(Component::Type::CANVAS_RENDERER);
                game_object->CreateComponent(Component::Type::BUTTON);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Image"))
            {
                game_object->CreateComponent(Component::Type::TRANSFORM_2D);
                game_object->CreateComponent(Component::Type::CANVAS_RENDERER);
                game_object->CreateComponent(Component::Type::IMAGE);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Progress Bar"))
            {
                game_object->CreateComponent(Component::Type::PROGRESS_BAR);
                game_object->CreateComponent(Component::Type::TRANSFORM_2D);
                GameObject* background = new GameObject(game_object, "Bar Background");
                background->CreateComponent(Component::Type::TRANSFORM_2D);
                background->CreateComponent(Component::Type::CANVAS_RENDERER);
                background->CreateComponent(Component::Type::IMAGE);
                GameObject* fill = new GameObject(game_object, "Bar Fill");
                fill->CreateComponent(Component::Type::TRANSFORM_2D);
                fill->CreateComponent(Component::Type::CANVAS_RENDERER);
                fill->CreateComponent(Component::Type::IMAGE);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::EndPopup();
    }
}
