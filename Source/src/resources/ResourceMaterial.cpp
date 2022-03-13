#include "core/hepch.h"
#include "ResourceMaterial.h"

Hachiko::ResourceMaterial::ResourceMaterial(UID uid) :
    Resource(uid, Type::MATERIAL) {}

Hachiko::ResourceMaterial::~ResourceMaterial()
{
}
