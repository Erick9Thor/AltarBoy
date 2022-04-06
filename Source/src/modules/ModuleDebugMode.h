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
		ModuleDebugMode() : is_gui_active(false), fps(0), poly_on_screen(0), poly_total(0), player_pos(0, 0, 0),
							window_flags(ImGuiWindowFlags_None), hw_info(), player(nullptr) {};
		~ModuleDebugMode() = default;

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
		const GameObject* FindPlayer() const;

		//TODO: Remove ASAP
		void RenderGui() const;
	private:
		bool is_gui_active;
		int fps;
		unsigned poly_on_screen;
		unsigned poly_total;
		std::vector<float> fps_buffer;
		std::vector<float> ms_buffer;
		float3 player_pos;
		HardwareInfo hw_info;
		ImGuiWindowFlags window_flags;
		//Deprecated after VS1
		const GameObject* player;
	};
}


