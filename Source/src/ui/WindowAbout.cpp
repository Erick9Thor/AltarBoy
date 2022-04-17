#include "core/hepch.h"
#include "WindowAbout.h"

#include "utils/Hardware.h"

Hachiko::WindowAbout::WindowAbout() :
    Window("About", false)
{
    hw = new Hardware();
}

Hachiko::WindowAbout::~WindowAbout()
{
    delete hw;
}

void Hachiko::WindowAbout::Update()
{
    ImGui::SetNextWindowSize(ImVec2(400.0f, 200.0f), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(name, &active, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Engine name: %s", TITLE);
        ImGui::Text("Version: %s", ENGINE_VERSION);
        ImGui::Separator();
        ImGui::Text("Akita Interactive Studios");
        ImGui::Text("Master in Advanced Programming for AAA Video Games UPC.");
        ImGui::Text("Engine licensed under the MIT License.");
        ImGui::Separator();

        auto child_size = ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 6);

        ImGui::Text("SDL Version: %d.%d.%d", hw->GetInfo().sdl_version.major, hw->GetInfo().sdl_version.minor, hw->GetInfo().sdl_version.patch);
        ImGui::Separator();
        ImGui::Text("CPUs: %d", hw->GetInfo().n_cpu);
        ImGui::Text("System RAM: %.1f Gb", hw->GetInfo().ram_gb);
        ImGui::Separator();
        ImGui::Text("GPU: %s", hw->GetInfo().gpu);
        ImGui::Text("Brand: %s", hw->GetInfo().gpu_brand);
        ImGui::Text("VRAM Budget: %.1f Mb", static_cast<float>(hw->GetInfo().vram_mb_budget));
        ImGui::Text("Vram Avaliable:  %.1f Mb", static_cast<float>(hw->GetInfo().vram_mb_free));
        ImGui::Text("Vram Usage:  %.1f Mb", (float)(hw->GetInfo().vram_mb_budget) - (float)(hw->GetInfo().vram_mb_free));
        ImGui::Separator();
        ImGui::Text("DevIL lib version %d", IL_VERSION);
    }
    ImGui::End();
}
