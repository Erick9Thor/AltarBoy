#include "ResourceMaterial.h"
#include "../Application.h"

ResourceMaterial::ResourceMaterial(UID uid)
	: Resource(uid, ResourceType::MATERIAL)
{
	library_path = LIBRARY_MATERIAL_FOLDER;
}

ResourceMaterial::~ResourceMaterial()
{
	App->texture->Unload(diffuse);
	App->texture->Unload(specular);
}