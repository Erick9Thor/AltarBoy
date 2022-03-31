#pragma once

#include "core/hepch.h"

#include "Application.h"
#include "ModuleInput.h"

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

		[[nodiscard]] bool IsActive() const { return is_gui_active; }
		void Toggle() { is_gui_active = !is_gui_active; }

	private:
		void DrawGUI();
	private:
		bool is_gui_active;
	};
}


