#include "WindowProject.h"

#include "imgui.h"
#include <IconsFontAwesome5.h>


WindowProject::WindowProject()
	: Window("Project", true) {}

void WindowProject::Update()
{
	if (ImGui::Begin(ICON_FA_FOLDER "Project", &active))
	{
	}
	ImGui::End();
}

