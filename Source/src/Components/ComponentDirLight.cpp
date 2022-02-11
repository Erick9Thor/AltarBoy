#include "ComponentDirLight.h"

#include "UI/ImGuiUtils.h"

#include "ComponentTransform.h"

#include "Core/Scene.h"

#include "imgui.h"
#include <algorithm>

#include "Core/GameObject.h"

#include <debugdraw.h>

Hachiko::ComponentDirLight::ComponentDirLight(GameObject* conatiner) :
    Component(Type::DIRLIGHT, conatiner)
{
    if (game_object->scene_owner)
    {
        game_object->scene_owner->dir_lights.push_back(this);
    }
}

Hachiko::ComponentDirLight::~ComponentDirLight()
{
    if (game_object->scene_owner)
    {
        auto& lights = game_object->scene_owner->dir_lights;
        lights.erase(std::remove(lights.begin(), lights.end(), this), lights.end());
    }
}

void Hachiko::ComponentDirLight::DebugDraw()
{
    if (draw_direction)
    {
        auto* transform = game_object->GetComponent<ComponentTransform>();
        if (transform)
        {
            dd::arrow(transform->GetPosition(), transform->GetFwd().Mul(3.f) + transform->GetPosition(), dd::colors::Blue, 1.f);
        }
    }
}

float3 Hachiko::ComponentDirLight::GetDirection() const
{
    const ComponentTransform* transform = game_object->GetComponent<ComponentTransform>();
    return transform->GetFwd();
}

void Hachiko::ComponentDirLight::Save(JsonFormatterValue j_component) const
{
    j_component["LightType"] = static_cast<int>(Type::DIRLIGHT);

    const JsonFormatterValue j_color = j_component["Color"];
    j_color[0] = color.x;
    j_color[1] = color.y;
    j_color[2] = color.z;
    j_color[3] = color.w;

    j_component["intensity"] = intensity;
    j_component["active"] = active;
    j_component["drawDirection"] = draw_direction;
}

void Hachiko::ComponentDirLight::Load(JsonFormatterValue j_component)
{
    const JsonFormatterValue j_color = j_component["Color"];
    color = float4(j_color[0], j_color[1], j_color[2], j_color[3]);

    intensity = j_component["intensity"];
    draw_direction = j_component["drawDirection"];
}

void Hachiko::ComponentDirLight::DrawGui()
{
    ImGui::PushID(this);
    if (ImGuiUtils::CollapsingHeader(game_object, this, "Dir Light"))
    {
        ImGui::Checkbox("D.Active", &active);
        ImGui::Checkbox("Draw Direction", &draw_direction);
        ImGui::PushItemWidth(100.0f);
        ImGui::InputFloat("D.Intensity", &intensity);
        ImGui::PopItemWidth();
        ImGuiUtils::CompactColorPicker("Dir Color", &color[0]);
    }
    ImGui::PopID();
}
