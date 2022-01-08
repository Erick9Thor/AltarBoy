#include "WindowResource.h"

#include "imgui.h"
#include <IconsFontAwesome5.h>

WindowResource::WindowResource()
	: Window("Resources", true) {}


void WindowResource::Update()
{
	if (ImGui::Begin(ICON_FA_FILE "Resource", &active))
	{
	}
	ImGui::End();
}
