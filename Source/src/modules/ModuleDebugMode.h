#pragma once

#include "core/hepch.h"

#include "Application.h"
#include "utils/Timer.h"

#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "RenderList.h"
//Delete after  VS1
#include "ModuleSceneManager.h"
#include "components/ComponentTransform.h"



struct HardwareInfo {

	unsigned char* system;
	int cpu;
	float ram;
	unsigned char* gpu;
	unsigned char* gpu_vendor;
	int vram_capacity;
	int vram_free;
	SDL_version sdl_version;
};

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
		ImGuiWindowFlags SetupWindow();
		void UpdateRenderValues();
		const GameObject* FindPlayer();
	private:
		bool is_gui_active;
		int fps, poly_on_screen, poly_total;
		std::vector<float> fps_buffer;
		std::vector<float> ms_buffer;
		float3 player_pos;
		HardwareInfo hw_info;
		ImGuiWindowFlags window_flags;
		//Deprecated after VS1
		const GameObject* player;
	};
}


