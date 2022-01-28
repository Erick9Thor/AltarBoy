#pragma once

#include "../Utils/JsonFormaterValue.h"

class ResourceMaterial;
class ResourceTexture;

namespace MaterialImporter
{
	namespace Material
	{
		void Import(const char* filePath, JsonFormaterValue json_meta);
		bool Save(const ResourceMaterial* resource_material, char** buffer);
		void Load(const char* buffer, ResourceMaterial* rMaterial);
	}
	
	namespace Textures
	{
		ResourceTexture* Import(const char* buffer);
		bool Save(const ResourceTexture* rTexture, char* file_path);
		ResourceTexture* Load(const char* file_path);
	}
}