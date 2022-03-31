#include "core/hepch.h"

#include "ModuleDebugMode.h"

Hachiko::ModuleDebugMode::ModuleDebugMode()
{
	is_gui_active = false;
}

Hachiko::ModuleDebugMode::~ModuleDebugMode()
{
	
}

bool Hachiko::ModuleDebugMode::Init()
{
	return true;
}

UpdateStatus Hachiko::ModuleDebugMode::PreUpdate(const float delta)
{
	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus Hachiko::ModuleDebugMode::Update(const float delta)
{
	if (App->input->GetKey(SDL_SCANCODE_LALT) == Hachiko::KeyState::KEY_DOWN &&
		App->input->GetKey(SDL_SCANCODE_E) == Hachiko::KeyState::KEY_DOWN)
	{
		Toggle();
	}

	if (is_gui_active)
	{
		DrawGUI();
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus Hachiko::ModuleDebugMode::PostUpdate(const float delta)
{
	return UpdateStatus::UPDATE_CONTINUE;
}

bool Hachiko::ModuleDebugMode::CleanUp()
{
	return true;
}


void Hachiko::ModuleDebugMode::DrawGUI()
{
	// Use ImGui to draw the menu with all the options needed
	/*
		List of things we should put in this cheat mode

			- TODO: Make it available only on debug
			- TODO: Add toggle free camera movement that reverts back to original position when movement is disabled
			- TODO: Add game time control
	*/

	//glBindFramebuffer(GL_FRAMEBUFFER, App->renderer->GetFrameBuffer());
	//Imgui stuff

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (ImGui::Begin("InGame Window", &is_gui_active))
	{

	}
	ImGui::End();
}
