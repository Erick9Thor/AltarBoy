#include "ModuleDebugMode.h"

Hachiko::ModuleDebugMode::ModuleDebugMode()
{
	isActive = false;
	active_camera = nullptr;
}

Hachiko::ModuleDebugMode::~ModuleDebugMode()
{

}

bool Hachiko::ModuleDebugMode::Init()
{
	GetActiveCamera();
	return false;
}

UpdateStatus Hachiko::ModuleDebugMode::PreUpdate(const float delta)
{
	// Set up ImGUI in respective context/camera.
	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus Hachiko::ModuleDebugMode::Update(const float delta)
{
	if (ToggleKeysPressed())
	{
		Toggle();
	}

	if (isActive)
	{
		DrawGUI();
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus Hachiko::ModuleDebugMode::PostUpdate(const float delta)
{
	return UpdateStatus::UPDATE_CONTINUE;
}

void Hachiko::ModuleDebugMode::GetActiveCamera()
{
	// Get the camera FBO we are rendering the GUI into
	active_camera = &App->camera;
}

void Hachiko::ModuleDebugMode::DrawGUI()
{
	// Use ImGui to draw the menu with all the options needed
	/*
		List of things we should put in this cheat mode

			- TODO: Make it available only on debug
			- TODO: Add toggle free camera movement that reverts back to original position when movement is disabled
			- TODO: Add game time control
			- TODO: Add button to kill player
			- TODO: Enable player god mode option
			- TODO: Button to spawn enemy in front
			- TODO: Show current coordinates and option to teleport player to different ones

	*/

	assert(active_camera != nullptr);
}
