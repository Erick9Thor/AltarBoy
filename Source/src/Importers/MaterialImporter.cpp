#include "MaterialImporter.h"

#include "../Application.h"
#include "../Modules/ModuleTexture.h"
#include "../Resources/ResourceMaterial.h"

void ColorCopy(const aiColor4D& assimp_color, float4& color)
{
	color.x = assimp_color.r;
	color.y = assimp_color.g;
	color.z = assimp_color.b;
	color.w = assimp_color.a;
}

ResourceMaterial* MaterialImporter::Material::Import(const aiMaterial* assimp_material, const std::string& model_path, const std::string& model_name)
{
	static const int index = 0;
	static const std::string default_path(ASSETS_FOLDER "\\Textures\\");
	aiString file;

	ResourceMaterial* r_material = new ResourceMaterial(0);

	aiColor4D color;
	if (assimp_material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
	{
		ColorCopy(color, r_material->diffuse_color);
	}
	if (assimp_material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
	{
		ColorCopy(color, r_material->specular_color);
	}

	assimp_material->Get(AI_MATKEY_SHININESS, r_material->shininess);

	std::vector<std::string> search_paths;
	if (assimp_material->GetTexture(aiTextureType_DIFFUSE, index, &file) == AI_SUCCESS)
	{
		std::string model_texture_path(file.data);
		std::string texture_file = model_texture_path.substr(model_texture_path.find_last_of("/\\") + 1);
		search_paths.push_back(file.data);
		search_paths.push_back(model_path + texture_file);
		search_paths.push_back(default_path + texture_file);
	}
	search_paths.push_back(model_path + model_name + "Diffuse.png");
	search_paths.push_back(default_path + model_name + "Diffuse.png");

	for (std::string path : search_paths)
	{
		r_material->diffuse = App->texture->Load(path.c_str());
		if (r_material->diffuse.loaded)
			break;
	}

	search_paths.clear();
	if (assimp_material->GetTexture(aiTextureType_SPECULAR, index, &file) == AI_SUCCESS)
	{
		std::string model_texture_path(file.data);
		std::string texture_file = model_texture_path.substr(model_texture_path.find_last_of("/\\") + 1);
		search_paths.push_back(file.data);
		search_paths.push_back(model_path + texture_file);
		search_paths.push_back(default_path + texture_file);
	}
	search_paths.push_back(model_path + model_name + "Specular.tif");
	search_paths.push_back(default_path + model_name + "Specular.tif");

	for (std::string path : search_paths)
	{
		// Not flip specular because devil seems to auto flip tif images already
		constexpr bool flip = false;
		r_material->specular = App->texture->Load(path.c_str(), flip);
		if (r_material->specular.loaded)
			break;
	}
	return r_material;
}

bool MaterialImporter::Material::Save(const ResourceMaterial* material, JsonFormaterValue j_material)
{
	j_material["HasDiffuse"] = material->diffuse.loaded;
	j_material["HasSpecular"] = material->specular.loaded;
	j_material["DiffusePath"] = material->diffuse.path.c_str();
	j_material["SpecularPath"] = material->specular.path.c_str();
	j_material["Shininess"] = material->shininess;

	JsonFormaterValue j_diffuse_color = j_material["DiffuseColor"];
	j_diffuse_color[0] = material->diffuse_color.x;
	j_diffuse_color[1] = material->diffuse_color.y;
	j_diffuse_color[2] = material->diffuse_color.z;
	j_diffuse_color[3] = material->diffuse_color.w;

	JsonFormaterValue j_specular_color = j_material["SpecularColor"];
	j_specular_color[0] = material->specular_color.x;
	j_specular_color[1] = material->specular_color.y;
	j_specular_color[2] = material->specular_color.z;	
	j_specular_color[3] = material->specular_color.w;
	return true;
}

ResourceMaterial* MaterialImporter::Material::Load(JsonFormaterValue j_material)
{
	// TODO: Manage uid properly
	ResourceMaterial* material = new ResourceMaterial(1);
	JsonFormaterValue j_diffuse_color = j_material["DiffuseColor"];
	JsonFormaterValue j_specular_color = j_material["SpecularColor"];
	material->diffuse_color = float4(j_diffuse_color[0], j_diffuse_color[1], j_diffuse_color[2], j_diffuse_color[3]);
	material->specular_color = float4(j_specular_color[0], j_specular_color[1], j_specular_color[2], j_specular_color[3]);

	//r_material->diffuse = App->texture->Load(path.c_str())

	std::string diffuse_path = j_material["DiffusePath"];
	material->diffuse = App->texture->Load(diffuse_path.c_str());
	std::string specular_path = j_material["SpecularPath"];
	material->specular = App->texture->Load(specular_path.c_str());
	material->shininess = j_material["Shininess"];
	return material;
}
