#include "core/hepch.h"
#include "WindowResource.h"

Hachiko::WindowResource::WindowResource() :
    Window("Resources", true) {}

Hachiko::WindowResource::~WindowResource() = default;

void Hachiko::WindowResource::Update()
{
    if (ImGui::Begin(ICON_FA_FILE "Resource", &active)) { }
    ImGui::End();
}
