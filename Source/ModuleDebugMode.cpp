#include "ModuleDebugMode.h"

Hachiko::ModuleDebugMode::ModuleDebugMode()
{
	isActive = false;
	target_fbo = 0;
}

Hachiko::ModuleDebugMode::~ModuleDebugMode()
{

}

bool Hachiko::ModuleDebugMode::Init()
{
	return false;
}

UpdateStatus Hachiko::ModuleDebugMode::Update(const float delta)
{
	if (ToggleDebugShortcut())
	{
		Toggle();
	}

	if (isActive)
	{
		DrawGUI();
	}
}

void Hachiko::ModuleDebugMode::GetRenderTarget()
{
	// Get the camera FBO we are rendering the GUI into
}

void Hachiko::ModuleDebugMode::DrawGUI()
{
	assert(target_fbo != 0);
	// Use ImGui to draw the menu with all the options needed
}
