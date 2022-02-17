#include "core/hepch.h"
#include "WindowAbout.h"

#include "modules/ModuleHardware.h"

Hachiko::WindowAbout::WindowAbout() :
    Window("About", false) {}

Hachiko::WindowAbout::~WindowAbout() = default;

void Hachiko::WindowAbout::Update()
{
    ImGui::SetNextWindowSize(ImVec2(400.0f, 200.0f), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(name, &active, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Engine name: %s", TITLE);
        ImGui::Text("Version: %s", ENGINE_VERSION);
        ImGui::Separator();
        ImGui::Text("Eric Torres Perramon & Bernat UWU");
        ImGui::Text("Master in Advanced Programming for AAA Video Games UPC.");
        ImGui::Text("Engine licensed under the MIT License.");
        ImGui::Separator();

        auto child_size = ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 6);

        ImGui::Text("SDL Version: %d.%d.%d", App->hw->GetInfo().sdl_version.major, App->hw->GetInfo().sdl_version.minor, App->hw->GetInfo().sdl_version.patch);
        ImGui::Separator();
        ImGui::Text("CPUs: %d", App->hw->GetInfo().n_cpu);
        ImGui::Text("System RAM: %.1f Gb", App->hw->GetInfo().ram_gb);
        ImGui::Separator();
        ImGui::Text("GPU: %s", App->hw->GetInfo().gpu);
        ImGui::Text("Brand: %s", App->hw->GetInfo().gpu_brand);
        ImGui::Text("VRAM Budget: %.1f Mb", static_cast<float>(App->hw->GetInfo().vram_mb_budget));
        ImGui::Text("Vram Avaliable:  %.1f Mb", static_cast<float>(App->hw->GetInfo().vram_mb_free));
        ImGui::Text("Vram Usage:  %.1f Mb", static_cast<float>(App->hw->GetInfo().vram_mb_budget - App->hw->GetInfo().vram_mb_free));
        ImGui::Separator();
        ImGui::Text("DevIL lib version %d", IL_VERSION);
    }
    ImGui::End();
}
