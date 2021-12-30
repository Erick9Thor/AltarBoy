#include "WindowConfiguration.h"

#include "../Application.h"
#include "../Modules/ModuleEditor.h"

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
		if (ImGui::CollapsingHeader("Scene")) {
		}
		if (ImGui::CollapsingHeader("Engine Camera"))
		{
		}
	}
	ImGui::End();
}
