#include "core/hepch.h"
#include "ComponentPointLight.h"
#include "ComponentTransform.h"

#include <debugdraw.h>

Hachiko::ComponentPointLight::ComponentPointLight(GameObject* conatiner) :
    Component(Type::POINTLIGHT, conatiner)
{
    if (game_object->scene_owner)
    {
        game_object->scene_owner->point_lights.push_back(this);
    }
}

Hachiko::ComponentPointLight::~ComponentPointLight()
{
    if (game_object->scene_owner)
    {
        auto& lights = game_object->scene_owner->point_lights;
        lights.erase(std::remove(lights.begin(), lights.end(), this), lights.end());
    }
}

void Hachiko::ComponentPointLight::DebugDraw()
{
    if (draw_sphere)
    {
        auto* transform = game_object->GetTransform();
        if (transform)
        {
            dd::sphere(transform->GetPosition(), dd::colors::Blue, radius);
        }
    }
}

float3 Hachiko::ComponentPointLight::GetPosition() const
{
    const ComponentTransform* transform = game_object->GetTransform();
    return transform->GetPosition();
}

void Hachiko::ComponentPointLight::Save(JsonFormatterValue j_component) const
{
    j_component["LightType"] = static_cast<int>(Type::POINTLIGHT);

    const JsonFormatterValue j_color = j_component["Color"];
    j_color[0] = color.x;
    j_color[1] = color.y;
    j_color[2] = color.z;
    j_color[3] = color.w;

    j_component["intensity"] = intensity;
    j_component["radius"] = radius;
    j_component["active"] = active;
    j_component["drawSphere"] = draw_sphere;
}

void Hachiko::ComponentPointLight::Load(JsonFormatterValue j_component)
{
    const JsonFormatterValue j_color = j_component["Color"];
    color = float4(j_color[0], j_color[1], j_color[2], j_color[3]);

    intensity = j_component["intensity"];
    radius = j_component["radius"];
    active = j_component["active"];
    draw_sphere = j_component["drawSphere"];
}

void Hachiko::ComponentPointLight::DrawGui()
{
    ImGui::PushID(this);
    if (ImGuiUtils::CollapsingHeader(game_object, this, "Point Light"))
    {
        ImGui::Checkbox("P.Active", &active);
        ImGui::Checkbox("Draw Sphere", &draw_sphere);
        ImGui::PushItemWidth(100.0f);
        ImGui::InputFloat("P.Intensity", &intensity);
        ImGui::InputFloat("P.Radius", &radius);
        ImGui::PopItemWidth();
        ImGuiUtils::CompactColorPicker("Point Color", &color[0]);
    }
    ImGui::PopID();
}
