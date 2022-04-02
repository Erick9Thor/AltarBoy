#include "core/hepch.h"
#include "WindowConfiguration.h"

#include "modules/ModuleEditor.h"
#include "modules/ModuleProgram.h"
#include "modules/ModuleCamera.h"
#include "modules/ModuleWindow.h"

#include "components/ComponentCamera.h"

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

        // TODO: REMOVE
        if (ImGui::Button("Toggle editor camera"))
        {
            App->camera->ActivateEditorCamera();
        }
        //

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
