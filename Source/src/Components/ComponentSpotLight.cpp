#include "ComponentSpotLight.h"

#include "UI/ImGuiUtils.h"

#include "ComponentTransform.h"
#include "Core/Scene.h"
#include "debugdraw.h"

#include "imgui.h"
#include <algorithm>

ComponentSpotLight::ComponentSpotLight(GameObject* conatiner)
	: Component(Component::Type::SPOTLIGHT, conatiner)
{
	if (game_object->scene_owner)
		game_object->scene_owner->spot_lights.push_back((ComponentSpotLight*) this);
}

ComponentSpotLight::~ComponentSpotLight()
{
	if (game_object->scene_owner)
	{
		auto& lights = game_object->scene_owner->spot_lights;
		lights.erase(std::remove(lights.begin(), lights.end(), this), lights.end());
	}
}

void ComponentSpotLight::DebugDraw()
{
	if (draw_cone)
	{
		ComponentTransform* transform = game_object->GetComponent<ComponentTransform>();
		if (transform)
		{
			dd::cone(transform->GetPosition(), transform->GetFwd().Mul(radius), dd::colors::Blue, inner, 0.f);
			dd::cone(transform->GetPosition(), transform->GetFwd().Mul(radius), dd::colors::Green, outer, 0.f);
		}
	}
}

float3 ComponentSpotLight::GetPosition()
{
	ComponentTransform* transform = game_object->GetComponent<ComponentTransform>();
	return transform->GetPosition();
}

float3 ComponentSpotLight::GetDirection()
{
	ComponentTransform* transform = game_object->GetComponent<ComponentTransform>();
	return transform->GetFwd();
}

void ComponentSpotLight::Save(JsonFormaterValue j_component) const
{
	j_component["LightType"] = (int) Component::Type::SPOTLIGHT;

	JsonFormaterValue j_color = j_component["Color"];
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

void ComponentSpotLight::Load(JsonFormaterValue j_component)
{
	JsonFormaterValue j_color = j_component["Color"];
	color = float4(j_color[0], j_color[1], j_color[2], j_color[3]);

	inner = j_component["inner"];
	outer = j_component["outer"];

	intensity = j_component["intensity"];
	radius = j_component["radius"];

	active = j_component["active"];
	draw_cone = j_component["drawSphere"];
}

void ComponentSpotLight::DrawGui()
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