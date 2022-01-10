#include "ComponentMaterial.h"

#include "../Application.h"
#include "../Modules/ModuleSceneManager.h"
#include "../Scene.h"

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

void ComponentMaterial::Save(JsonFormaterValue jComponent) const
{
	jComponent["TextureId"] = texture.id;
}

void ComponentMaterial::Load(JsonFormaterValue jComponent)
{
	unsigned int material_id = jComponent["TextureId"];
	// texture = App->scene_manager->GetActiveScene().m_textures[material_id];
}
