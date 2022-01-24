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

/*
//	--- Texture importer --- //

void MaterialImporter::Textures::Init()
{
}

ResourceTexture* MaterialImporter::Textures::Create()
{
	return new ResourceTexture();
}

bool MaterialImporter::Textures::Import(const char* buffer, ResourceTexture* rTexture)
{
	return false;
}

bool MaterialImporter::Textures::Save(const ResourceTexture* rTexture, char** buffer)
{
	return false;
}

void MaterialImporter::Textures::Load(const char* buffer, ResourceTexture* texture)
{
}*/
