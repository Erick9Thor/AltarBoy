#include "WindowConfiguration.h"

#include "../Application.h"
#include "../Scene.h"
#include "../Modules/ModuleEditor.h"
#include "../Modules/ModuleSceneManager.h"
#include "../Modules/ModuleProgram.h"
#include "../Modules/ModuleCamera.h"
#include "../Components/ComponentCamera.h"

#include <imgui.h>
#include <IconsFontAwesome5.h>

WindowConfiguration::WindowConfiguration()
	: Window("Configuration", true)
{
}

WindowConfiguration::~WindowConfiguration()
{
}

void WindowConfiguration::Update()
{
	if (ImGui::Begin(ICON_FA_COG "Configuration", &active))
	{
		if (ImGui::CollapsingHeader("Scene"))
		{
			App->renderer->OptionsMenu();
			ImGui::Separator();
			ImGui::Text("Shader Options");
			App->program->OptionsMenu();
		}
		if (ImGui::CollapsingHeader("Camera"))
		{
			App->camera->GetMainCamera()->DrawGui();
		}
		if (ImGui::CollapsingHeader("Engine"))
		{
			App->window->OptionsMenu();
			App->renderer->PerformanceMenu();
		}
	}
	ImGui::End();
}
