#include "ResourceMaterial.h"
#include "../Application.h"

ResourceMaterial::ResourceMaterial(UID uid)
	: Resource(uid, ResourceType::MATERIAL)
{
}

ResourceMaterial::~ResourceMaterial()
{
}