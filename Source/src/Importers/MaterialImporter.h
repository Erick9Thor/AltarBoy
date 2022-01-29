#pragma once

#include "../Utils/JsonFormaterValue.h"
#include "../Utils/UID.h"

#include "assimp/scene.h"

class ResourceMaterial;
class ResourceTexture;

namespace MaterialImporter
{
	namespace Material
	{
		ResourceMaterial* Import(const aiMaterial* assimp_material, const std::string& model_path, const std::string& model_name);
		bool Save(const ResourceMaterial* material, JsonFormaterValue j_material);
		ResourceMaterial* Load(JsonFormaterValue j_material);
	}
	
	namespace Textures
	{
		ResourceTexture* Import(const char* buffer);
		bool Save(const ResourceTexture* rTexture, char* file_path);
		ResourceTexture* Load(const char* file_path);
	}
}