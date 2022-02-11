#include "ComponentPointLight.h"

#include "UI/ImGuiUtils.h"

#include "ComponentTransform.h"
#include "Core/Scene.h"
#include "debugdraw.h"

#include <IconsFontAwesome5.h>

#include "imgui.h"

#include <algorithm>

ComponentPointLight::ComponentPointLight(GameObject* conatiner)
	: Component(Component::Type::POINTLIGHT, conatiner)
{
	if (game_object->scene_owner)
		game_object->scene_owner->point_lights.push_back((ComponentPointLight*) this);
}

ComponentPointLight::~ComponentPointLight()
{
	if (game_object->scene_owner)
	{
		auto& lights = game_object->scene_owner->point_lights;
		lights.erase(std::remove(lights.begin(), lights.end(), this), lights.end());
	}
}

void ComponentPointLight::DebugDraw()
{
	if (draw_sphere)
	{
		ComponentTransform* transform = game_object->GetComponent<ComponentTransform>();
		if (transform)
		{
			dd::sphere(transform->GetPosition(), dd::colors::Blue, radius);
		}
	}
}

float3 ComponentPointLight::GetPosition()
{
	ComponentTransform* transform = game_object->GetComponent<ComponentTransform>();
	return transform->GetPosition();
}

void ComponentPointLight::Save(JsonFormaterValue j_component) const
{
	j_component["LightType"] = (int) Component::Type::POINTLIGHT;

	JsonFormaterValue j_color = j_component["Color"];
	j_color[0] = color.x;
	j_color[1] = color.y;
	j_color[2] = color.z;
	j_color[3] = color.w;

	j_component["intensity"] = intensity;
	j_component["radius"] = radius;
	j_component["active"] = active;
	j_component["drawSphere"] = draw_sphere;
}

void ComponentPointLight::Load(JsonFormaterValue j_component)
{
	JsonFormaterValue j_color = j_component["Color"];
	color = float4(j_color[0], j_color[1], j_color[2], j_color[3]);

	intensity = j_component["intensity"];
	radius = j_component["radius"];
	active = j_component["active"];
	draw_sphere = j_component["drawSphere"];
}

void ComponentPointLight::DrawGui()
{
	ImGui::PushID(this);
	if (ImGuiUtils::CollapsingHeader(game_object, this, "Point Light")) {
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

