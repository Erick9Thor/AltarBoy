#include "WindowResource.h"

#include "imgui.h"
#include <IconsFontAwesome5.h>

WindowResource::WindowResource()
	: Window("Resources", true) {}

WindowResource::~WindowResource()
{
}


void WindowResource::Update()
{
	if (ImGui::Begin(ICON_FA_FILE "Resource", &active))
	{
	}
	ImGui::End();
}
