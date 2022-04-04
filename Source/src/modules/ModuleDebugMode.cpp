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
	
	hw_info.system = (unsigned char*)SDL_GetPlatform();
	hw_info.cpu = SDL_GetCPUCount();
	hw_info.ram = SDL_GetSystemRAM() / 1024.0f;
	hw_info.gpu = (unsigned char*)glGetString(GL_RENDERER);
	hw_info.gpu_vendor = (unsigned char*)glGetString(GL_VENDOR);
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &hw_info.vram_capacity);
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &hw_info.vram_free);
	SDL_GetVersion(&hw_info.sdl_version);
	return true;
}

UpdateStatus Hachiko::ModuleDebugMode::PreUpdate(const float delta)
{
	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus Hachiko::ModuleDebugMode::Update(const float delta)
{
	if (App->input->GetKey(SDL_SCANCODE_LALT) == Hachiko::KeyState::KEY_REPEAT &&
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
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	SDL_GL_MakeCurrent(App->window->GetWindow(), App->renderer->GetGLContext());

	return UpdateStatus::UPDATE_CONTINUE;
}

bool Hachiko::ModuleDebugMode::CleanUp()
{
	return true;
}

ImGuiWindowFlags Hachiko::ModuleDebugMode::SetupWindow()
{


	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
	return window_flags;
}

void Hachiko::ModuleDebugMode::UpdateRenderValues()
{
	fps = EngineTimer::fps;
}

void Hachiko::ModuleDebugMode::DrawGUI()
{
	static SDL_version version;
	static const float vram_total = hw_info.vram_capacity / 1024.0f;
	float vram_free = hw_info.vram_free / 1024.0f;
	float vram_usage = vram_total - vram_free;
	float frame_rate = 0.0f;
	
	window_flags = SetupWindow();
	UpdateRenderValues();

	if (ImGui::Begin("InGame Window", &is_gui_active, window_flags))
	{
		ImGui::Text("FPS: %i", fps);
		//char title[25];
		//sprintf_s(title, 25, "Framerate %.1f", frame_rate);
		//ImGui::PlotHistogram("##framerate", &fps_buffer[0], (int)fps_buffer.size(), 0, title, 0.0f, 1000.f, ImVec2(310, 100));
		ImGui::Separator();

		ImGui::Text("Polycount (On screen): %i", poly_on_screen);
		ImGui::Text("Polycount (Total loaded): %i", poly_total);
		ImGui::Separator();
		ImGui::Text("CPUs: %d", hw_info.cpu);
		ImGui::Text("System RAM: %.1f Gb", hw_info.ram);
		ImGui::Text("GPU: %s", hw_info.gpu);
		ImGui::Text("Vendor: %s", hw_info.gpu_vendor);
		ImGui::Text("VRAM: %.1f Mb", vram_free);
		ImGui::Text("Vram Usage:  %.1f Mb", vram_usage);
		ImGui::Text("Vram Available:  %.1f Mb", vram_free);
		ImGui::Separator();

		if(ImGui::Button("Toggle Solid"))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		ImGui::SameLine();
		if(ImGui::Button("Toggle Wire"))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
	ImGui::End();
}
