#include "WindowConfiguration.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"
#include "Modules/ModuleProgram.h"
#include "Modules/ModuleCamera.h"
#include "Components/ComponentCamera.h"

#include <imgui.h>
#include <IconsFontAwesome5.h>

#include "Modules/ModuleWindow.h"

Hachiko::WindowConfiguration::WindowConfiguration() :
    Window("Configuration", true) {}

Hachiko::WindowConfiguration::~WindowConfiguration() = default;

void Hachiko::WindowConfiguration::Update()
{
    ImGui::SetNextWindowDockID(App->editor->dock_right_id, ImGuiCond_FirstUseEver);
    if (ImGui::Begin((std::string(ICON_FA_COG " ") + name).c_str(), &active))
    {
        if (ImGui::CollapsingHeader("Scene"))
        {
            App->renderer->OptionsMenu();
            ImGui::Separator();
            ImGui::Text("Shader Options");
            App->program->OptionsMenu();
        }

        //It is already a collapsing header
        App->camera->GetMainCamera()->DrawGui();

        if (ImGui::CollapsingHeader("Engine"))
        {
            App->window->OptionsMenu();
            App->renderer->PerformanceMenu();
        }
    }
    ImGui::End();
}
