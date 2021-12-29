#include "ComponentMaterial.h"
#include <imgui.h>

ComponentMaterial::ComponentMaterial(GameObject* conatiner)
	: Component(Component::Type::Material, conatiner)
{
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::DrawGui()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Diffuse: %dx%d %s", texture.width, texture.height, texture.path.c_str());
		ImGui::Image((void*) (intptr_t) texture.id, ImVec2(150, 150));
	}
}