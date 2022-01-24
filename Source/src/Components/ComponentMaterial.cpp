#include "ComponentMaterial.h"

#include "../Application.h"
#include "../Modules/ModuleTexture.h"
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
		if (material) {
			Texture& diffuse = material->diffuse;
			if (diffuse.loaded)
			{
				ImGui::Text("Diffuse: %dx%d %s", diffuse.width, diffuse.height, diffuse.path.c_str());
				ImGui::Image((void*) (intptr_t) diffuse.id, ImVec2(150, 150));
			}

			Texture& specular = material->specular;
			if (specular.loaded)
			{
				ImGui::Text("Specular: %dx%d %s", specular.width, specular.height, specular.path.c_str());
				ImGui::Image((void*) (intptr_t) specular.id, ImVec2(150, 150));
			}
		}
		else
		{
			ImGui::Text("No material resource");
		}
		
	}
			
}

void ComponentMaterial::Save(JsonFormaterValue j_component) const
{
	j_component["TextureId"] = material->diffuse.id;
}

void ComponentMaterial::Load(JsonFormaterValue j_component)
{
	unsigned int material_id = j_component["TextureId"];
	// texture = App->scene_manager->GetActiveScene().m_textures[material_id];
}
