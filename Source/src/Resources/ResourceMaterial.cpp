#include "ResourceMaterial.h"
#include "../Application.h"

ResourceMaterial::ResourceMaterial(UID uid)
	: Resource(uid, ResourceType::MATERIAL)
{
}

ResourceMaterial::~ResourceMaterial()
{
}

void ColorCopy(const aiColor4D& assimp_color, float4& color)
{
	color.x = assimp_color.r;
	color.y = assimp_color.g;
	color.z = assimp_color.b;
	color.w = assimp_color.a;
}

bool ResourceMaterial::Import(const aiMaterial* assimp_material, const std::string& model_path, const std::string& model_name)
{
	static const int index = 0;
	static const std::string default_path(ASSETS_FOLDER "\\Textures\\");
	aiString file;

	aiColor4D color;
	if (assimp_material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
	{
		ColorCopy(color, diffuse_color);
	}
	if (assimp_material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
	{
		ColorCopy(color, specular_color);
	}

	assimp_material->Get(AI_MATKEY_SHININESS, shininess);

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
		diffuse = App->texture->Load(path.c_str());
		if (diffuse.loaded)
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
		specular = App->texture->Load(path.c_str(), flip);
		if (specular.loaded)
			break;
	}
	return true;
}
