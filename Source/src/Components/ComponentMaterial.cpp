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

void ComponentMaterial::SetMaterial(ResourceMaterial* new_material)
{
	material = new_material;
	use_diffuse_texture = material->diffuse.loaded;
	use_specular_texture = material->specular.loaded;
}

void ComponentMaterial::DrawGui()
{
	constexpr ImGuiColorEditFlags flag = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel;
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (material) {
			Texture& diffuse = material->diffuse;
			if (diffuse.loaded)
			{
				ImGui::Checkbox("Diff Texture", &use_diffuse_texture);
				if (use_diffuse_texture)
				{
					ImGui::Text("Diffuse: %dx%d %s", diffuse.width, diffuse.height, diffuse.path.c_str());
					ImGui::Image((void*) (intptr_t) diffuse.id, ImVec2(150, 150));	
				}
			}
			if (!use_diffuse_texture)
				ImGui::ColorPicker3("Diffuse Color", &material->diffuse_color[0], flag);

			Texture& specular = material->specular;
			ImGui::InputFloat("Shininess", &material->shininess);
			if (specular.loaded)
			{
				ImGui::Checkbox("Spec Texture", &use_specular_texture);
				if (use_specular_texture)
				{
					ImGui::Text("Specular: %dx%d %s", specular.width, specular.height, specular.path.c_str());
					ImGui::Image((void*) (intptr_t) specular.id, ImVec2(150, 150));
				}
			}
			if (!use_specular_texture)
				ImGui::ColorPicker3("Specular Color", &material->specular_color[0], flag);
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
