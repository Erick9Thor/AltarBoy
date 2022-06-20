#include "core/hepch.h"

#include "ComponentDirLight.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "modules/ModuleEvent.h"

#include <debugdraw.h>

Hachiko::ComponentDirLight::ComponentDirLight(GameObject* conatiner) :
    Component(Type::DIRLIGHT, conatiner)
{
    if (game_object->scene_owner)
    {
        game_object->scene_owner->dir_lights.push_back(this);

        UpdateFrustum(game_object->scene_owner->GetMainCamera());
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
        auto* transform = game_object->GetTransform();
        if (transform)
        {
            dd::arrow(transform->GetGlobalPosition(), transform->GetFront().Mul(3.f) + transform->GetGlobalPosition(), dd::colors::Blue, 1.f);
        }
    }
}

float3 Hachiko::ComponentDirLight::GetDirection() const
{
    ComponentTransform* transform = game_object->GetTransform();
    return transform->GetFront();
}

void Hachiko::ComponentDirLight::Save(YAML::Node& node) const
{
    node[LIGHT_TYPE] = static_cast<int>(Type::DIRLIGHT);
    node[LIGHT_COLOR] = color;
    node[LIGHT_INTENSITY] = intensity;
    node[LIGHT_DRAW_DIRECTION] = draw_direction;
}

void Hachiko::ComponentDirLight::Load(const YAML::Node& node)
{
    color = node[LIGHT_COLOR].as<float4>();
    intensity = node[LIGHT_INTENSITY].as<float>();
    draw_direction = node[LIGHT_DRAW_DIRECTION].as<bool>();
}

void Hachiko::ComponentDirLight::DrawGui()
{
    ImGui::PushID(this);
    if (ImGuiUtils::CollapsingHeader(game_object, this, "Dir Light"))
    {
        if (ImGui::Checkbox("D.Active", &active))
        {
            App->event->Publish(Event::Type::CREATE_EDITOR_HISTORY_ENTRY);
        }
        if (ImGui::Checkbox("Draw Direction", &draw_direction))
        {
            App->event->Publish(Event::Type::CREATE_EDITOR_HISTORY_ENTRY);
        }

        ImGui::PushItemWidth(100.0f);
        ImGui::InputFloat("D.Intensity", &intensity);
        CREATE_HISTORY_ENTRY_AFTER_EDIT()
        ImGui::PopItemWidth();
        ImGuiUtils::CompactColorPicker("Dir Color", &color[0]);
        CREATE_HISTORY_ENTRY_AFTER_EDIT()
    }
    ImGui::PopID();
}

void Hachiko::ComponentDirLight::UpdateFrustum(ComponentCamera* camera) 
{
    constexpr const size_t NUM_CORNERS = 8;
    float3 camera_frustum_corners[NUM_CORNERS];
    camera->GetFrustum().GetCornerPoints(camera_frustum_corners);

    // Calculate sphere center:
    float3 camera_frustum_sphere_center = float3::zero;
    for (size_t i = 0; i < NUM_CORNERS; ++i)
    {
        camera_frustum_sphere_center += camera_frustum_corners[i];
    }
    camera_frustum_sphere_center = camera_frustum_sphere_center / 8.0f;

    // Calculate the sphere radius, it's the maximum of distances between
    // center and frustum corners:
    float camera_frustum_sphere_radius = 0.0f;
    for (size_t i = 0; i < NUM_CORNERS; ++i)
    {
        camera_frustum_sphere_radius = std::max(
            camera_frustum_sphere_center.Distance(camera_frustum_corners[i]), 
            camera_frustum_sphere_radius);
    }

    // Set frustum kind:
    frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
    // Set the orthographic width and height of the light frustum to be the
    // diameter of enclosing sphere of camera frustum:
    float orthographic_size = camera_frustum_sphere_radius * 2.0f;
    frustum.SetOrthographic(orthographic_size, orthographic_size);
    // Set far plane to be same with orthographic width and height, and set
    // near to be 0:
    frustum.SetViewPlaneDistances(0.0f, orthographic_size);
    // Set the directional light direction:
    float3 direction = GetDirection().Normalized();
    // Calculate right to get up:
    float3 right = direction.Cross(float3::unitY).Normalized();
    // Set position, front and up of the light frustum:
    frustum.SetFrame(
        camera_frustum_sphere_center - direction * camera_frustum_sphere_radius,
        direction,
        right.Cross(direction).Normalized());
}