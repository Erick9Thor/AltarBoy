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
		auto lights = game_object->scene_owner->point_lights;
		lights.erase(std::remove(lights.begin(), lights.end(), this), lights.end());
	}
}

void ComponentPointLight::DrawGui()
{
	if (ImGui::CollapsingHeader("Point Light"))
	{
		ImGuiColorEditFlags flag = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel;
		ImGui::ColorPicker3("Color", &color[0], flag);
	}
}

float3 ComponentPointLight::GetPosition()
{
	ComponentTransform* transform = game_object->GetComponent<ComponentTransform>();
	return transform->GetPosition();
}


