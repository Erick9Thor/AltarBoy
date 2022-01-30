#include "ResourceMaterial.h"
#include "../Application.h"

ResourceMaterial::ResourceMaterial(UID uid)
	: Resource(uid, ResourceType::MATERIAL)
{
}

ResourceMaterial::~ResourceMaterial()
{
	App->texture->Unload(diffuse);
	App->texture->Unload(specular);
}