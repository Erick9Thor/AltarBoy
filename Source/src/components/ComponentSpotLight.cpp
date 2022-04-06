#include "core/hepch.h"
#include "ComponentSpotLight.h"
#include "ComponentTransform.h"

#include <debugdraw.h>

Hachiko::ComponentSpotLight::ComponentSpotLight(GameObject* conatiner) :
    Component(Type::SPOTLIGHT, conatiner)
{
    if (game_object->scene_owner)
    {
        game_object->scene_owner->spot_lights.push_back(this);
    }
}

Hachiko::ComponentSpotLight::~ComponentSpotLight()
{
    if (game_object->scene_owner)
    {
        auto& lights = game_object->scene_owner->spot_lights;
        lights.erase(std::remove(lights.begin(), lights.end(), this), lights.end());
    }
}

void Hachiko::ComponentSpotLight::DebugDraw()
{
    if (draw_cone)
    {
        auto* transform = game_object->GetTransform();
        if (transform)
        {
            dd::cone(transform->GetLocalPosition(), transform->GetFront().Mul(radius), dd::colors::Blue, inner, 0.f);
            dd::cone(transform->GetLocalPosition(), transform->GetFront().Mul(radius), dd::colors::Green, outer, 0.f);
        }
    }
}

float3 Hachiko::ComponentSpotLight::GetPosition() const
{
    ComponentTransform* transform = game_object->GetTransform();
    return transform->GetLocalPosition();
}

float3 Hachiko::ComponentSpotLight::GetDirection() const
{
    ComponentTransform* transform = game_object->GetTransform();
    return transform->GetFront();
}

void Hachiko::ComponentSpotLight::Save(JsonFormatterValue j_component) const
{
    j_component["LightType"] = static_cast<int>(Type::SPOTLIGHT);

    const JsonFormatterValue j_color = j_component["Color"];
    j_color[0] = color.x;
    j_color[1] = color.y;
    j_color[2] = color.z;
    j_color[3] = color.w;

    j_component["inner"] = inner;
    j_component["outer"] = outer;

    j_component["intensity"] = intensity;
    j_component["radius"] = radius;

    j_component["active"] = active;
    j_component["drawCone"] = draw_cone;
}

void Hachiko::ComponentSpotLight::Load(JsonFormatterValue j_component)
{
    const JsonFormatterValue j_color = j_component["Color"];
    color = float4(j_color[0], j_color[1], j_color[2], j_color[3]);

    inner = j_component["inner"];
    outer = j_component["outer"];

    intensity = j_component["intensity"];
    radius = j_component["radius"];

    active = j_component["active"];
    draw_cone = j_component["drawSphere"];
}

void Hachiko::ComponentSpotLight::DrawGui()
{
    ImGui::PushID(this);
    if (ImGuiUtils::CollapsingHeader(game_object, this, "Spot Light"))
    {
        ImGui::PushItemWidth(100.0f);
        ImGui::Checkbox("S.Active", &active);
        ImGui::Checkbox("Draw Cone", &draw_cone);
        ImGui::InputFloat("S.Intensity", &intensity);
        ImGui::InputFloat("S.Radius", &radius);
        ImGui::InputFloat("Inner Angle", &inner);
        ImGui::InputFloat("Outer Angle", &outer);
        ImGui::PopItemWidth();
        ImGuiUtils::CompactColorPicker("Spot Color", &color[0]);
    }
    ImGui::PopID();
}
