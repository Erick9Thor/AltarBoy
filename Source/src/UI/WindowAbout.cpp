#include "WindowAbout.h"

#include "../Application.h"
#include "../Modules/ModuleHardware.h"


#include "imgui.h"
#include "../Globals.h"
#include <ilu.h>

WindowAbout::WindowAbout()
	: Window("About", false)
{
}

void WindowAbout::Update()
{
	if (ImGui::Begin(name, &active, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Engine name: %s", TITLE);
		ImGui::Text("Version: %s", ENGINE_VERSION);
		ImGui::Separator();
		ImGui::Text("Eric Torres Perramon & Bernat UWU");
		ImGui::Text("Master in Advanced Programming for AAA Video Games UPC.");
		ImGui::Text("Engine licensed under the MIT License.");
		ImGui::Separator();

		ImVec2 child_size = ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 6);

		ImGui::Text("SDL Version: %d.%d.%d", App->hw->GetInfo().sdl_version.major, App->hw->GetInfo().sdl_version.minor, App->hw->GetInfo().sdl_version.patch);
		ImGui::Separator();
		ImGui::Text("CPUs: %d", App->hw->GetInfo().n_cpu);
		ImGui::Text("System RAM: %.1f Gb", App->hw->GetInfo().ram_gb);
		ImGui::Separator();
		ImGui::Text("GPU: %s", App->hw->GetInfo().gpu);
		ImGui::Text("Brand: %s", App->hw->GetInfo().gpu_brand);
		ImGui::Text("VRAM Budget: %.1f Mb", (float) App->hw->GetInfo().vram_mb_budget);
		ImGui::Text("Vram Avaliable:  %.1f Mb", (float) App->hw->GetInfo().vram_mb_free);
		ImGui::Text("Vram Usage:  %.1f Mb", (float) (App->hw->GetInfo().vram_mb_budget - App->hw->GetInfo().vram_mb_free));
		ImGui::Separator();
		ImGui::Text("DevIL lib version %d", IL_VERSION);

		ImGui::EndChildFrame();
	}
	ImGui::End();
}
