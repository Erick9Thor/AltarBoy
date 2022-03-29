#pragma once

#include "core/hepch.h"

#include "Application.h"
#include "ModuleDebugMode.h"
#include "modules/ModuleInput.h"

constexpr bool ToggleDebugShortcut()
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
		UpdateStatus Update(const float delta) override;

		inline [[nodiscard]] bool IsActive() const { return isActive; };
		inline void Toggle() { isActive = !isActive; };

	private:
		void GetRenderTarget();
		void DrawGUI();
	private:
		bool isActive = false;
		int target_fbo = 0;
	};
}


