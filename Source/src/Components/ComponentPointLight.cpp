#include "ComponentPointLight.h"
#include "ComponentTransform.h"
#include "../Scene.h"

#include "imgui.h"

#include <algorithm>

ComponentPointLight::ComponentPointLight(GameObject* conatiner)
	: Component(Component::Type::PointLight, conatiner)
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

void ComponentPointLight::DrawGui()
{
	if (ImGui::CollapsingHeader("Point Light"))
	{
		ImGui::Checkbox("P.Active", &active);
		ImGui::PushItemWidth(100.0f);		
		ImGui::InputFloat("P.Intensity", &intensity);
		ImGui::InputFloat("P.Radius", &radius);
		ImGui::PopItemWidth();
		ImGuiUtils::CompactColorPicker("Point Color", &color[0]);
	}
}

float3 ComponentPointLight::GetPosition()
{
	ComponentTransform* transform = game_object->GetComponent<ComponentTransform>();
	return transform->GetPosition();
}


