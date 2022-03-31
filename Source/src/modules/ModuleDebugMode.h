#pragma once

#include "core/hepch.h"

#include "Application.h"
#include "ModuleDebugMode.h"
#include "modules/ModuleInput.h"

constexpr bool ToggleKeysPressed()
{
	return App->input->GetKey(SDL_SCANCODE_LALT) == Hachiko::KeyState::KEY_DOWN &&
		App->input->GetKey(SDL_SCANCODE_E) == Hachiko::KeyState::KEY_DOWN;
}

namespace Hachiko
{
	class ModuleDebugMode : public Module
	{
	public:
		ModuleDebugMode();
		~ModuleDebugMode();

		bool Init() override;
		UpdateStatus PreUpdate(const float delta) override;
		UpdateStatus Update(const float delta) override;
		UpdateStatus PostUpdate(const float delta) override;
		bool CleanUp() override;

		[[nodiscard]] bool IsActive() const { return isActive; }
		void Toggle() { isActive = !isActive; }

	private:
		void GetActiveCamera();
		void DrawGUI();
	private:
		bool isActive = false;
		ModuleCamera** active_camera = nullptr;
	};
}


