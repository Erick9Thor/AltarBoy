#include "MaterialImporter.h"

#include "../Resources/ResourceMaterial.h"
#include "../Resources/ResourceTexture.h"

void MaterialImporter::Material::Import(const char* filePath, JsonFormaterValue json_meta)
{
	
}

bool MaterialImporter::Material::Save(const ResourceMaterial* resource_material, char** buffer)
{
	return false;
}

void MaterialImporter::Material::Load(const char* buffer, ResourceMaterial* rMaterial)
{
}

ResourceTexture* MaterialImporter::Textures::Import(const char* buffer)
{
	return nullptr;
}

bool MaterialImporter::Textures::Save(const ResourceTexture* rTexture, char* file_path)
{
	return false;
}

ResourceTexture* MaterialImporter::Textures::Load(const char* file_path)
{
	return nullptr;
}
