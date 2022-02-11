#pragma once

#include "Utils/JsonFormaterValue.h"
#include "Utils/UUID.h"

#include "assimp/scene.h"

class ResourceMaterial;

namespace MaterialImporter
{
	namespace Material
	{
		ResourceMaterial* Import(const aiMaterial* assimp_material, const std::string& model_path, const std::string& model_name);
		bool Save(const ResourceMaterial* material, JsonFormaterValue j_material);
		ResourceMaterial* Load(JsonFormaterValue j_material);
	}
}