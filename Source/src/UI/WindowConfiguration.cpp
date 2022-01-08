#include "WindowConfiguration.h"

#include "../Application.h"
#include "../Scene.h"
#include "../Modules/ModuleEditor.h"
#include "../Modules/ModuleSceneManager.h"
#include "../Modules/ModuleCamera.h"
#include "../Components/ComponentCamera.h"

#include <imgui.h>
#include <IconsFontAwesome5.h>

WindowConfiguration::WindowConfiguration()
	: Window("Configuration", true)
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
			App->scene_manager->GetActiveScene()->OptionsMenu();
		}
		if (ImGui::CollapsingHeader("Engine Camera"))
		{
			App->camera->GetMainCamera()->DrawGui();
		}
		/*if (ImGui::CollapsingHeader("FPS control"))
		{
			App->renderer->FpsGraph();
		}*/
	}
	ImGui::End();
}
