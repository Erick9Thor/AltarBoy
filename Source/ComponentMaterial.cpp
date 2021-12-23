#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject* conatiner): Component(Component::Type::Material, conatiner)
{
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::DrawGui()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}