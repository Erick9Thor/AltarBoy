#pragma once

#include "../Utils/JsonFormaterValue.h"

class ResourceMaterial;
class ResourceTexture;

namespace MaterialImporter
{
	namespace Material
	{
		void Import(const char* filePath, JsonFormaterValue json_meta, ResourceMaterial* rMaterial);
		bool Save(const ResourceMaterial* resource_material, char** buffer);
		void Load(const char* buffer, ResourceMaterial* rMaterial);
	} // namespace Material
	/*
	namespace Textures
	{
		void Init();
		bool Import(const char* buffer, ResourceTexture* rTexture);
		bool Save(const ResourceTexture* rTexture, char** buffer);
		void Load(const char* buffer, ResourceTexture* texture);
	}
	*/
}