#include "ComponentSpotLight.h"

#include "ComponentTransform.h"
#include "../Scene.h"

#include "imgui.h"

#include <algorithm>

ComponentSpotLight::ComponentSpotLight(GameObject* conatiner)
	: Component(Component::Type::SpotLight, conatiner)
{
	if (game_object->scene_owner)
		game_object->scene_owner->spot_lights.push_back((ComponentSpotLight*) this);
}

ComponentSpotLight::~ComponentSpotLight()
{
	if (game_object->scene_owner)
	{
		auto lights = game_object->scene_owner->spot_lights;
		lights.erase(std::remove(lights.begin(), lights.end(), this), lights.end());
	}
}

void ComponentSpotLight::DrawGui()
{
	if (ImGui::CollapsingHeader("Spot Light"))
	{
		ImGui::Checkbox("S.Active", &active);
		ImGui::InputFloat("S.Intensity", &intensity);
		ImGui::InputFloat("S.Radius", &radius);
		ImGui::InputFloat("Inner Angle", &inner);
		ImGui::InputFloat("Outer Angle", &outer);
		ImGuiColorEditFlags flag = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel;
		ImGui::ColorPicker3("Spot Color", &color[0], flag);
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
