#include "ComponentMaterial.h"

#include "../Application.h"
#include "../Modules/ModuleTexture.h"
#include "../Modules/ModuleSceneManager.h"
#include "../Scene.h"

#include <imgui.h>

ComponentMaterial::ComponentMaterial(GameObject* conatiner)
	: Component(Component::Type::MATERIAL, conatiner)
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

void ComponentMaterial::Save(JsonFormaterValue j_component) const
{
	//TODO Save the path of the library model
	j_component["TextureId"] = material->diffuse.id;
	j_component["FileName"] = material->file_name.c_str();
}

void ComponentMaterial::Load(JsonFormaterValue j_component)
{
	unsigned int material_id = j_component["TextureId"];
	std::string file_name = j_component["FileName"];
	//Importer::MaterialImporter::Textures Load(file_name)
}

void ComponentMaterial::DrawGui()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (material)
		{
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
				ImGuiUtils::CompactColorPicker("Diffuse Color", &material->diffuse_color[0]);

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
				ImGuiUtils::CompactColorPicker("Specular Color", &material->specular_color[0]);
		}
		else
		{
			ImGui::Text("No material resource");
		}
	}
}