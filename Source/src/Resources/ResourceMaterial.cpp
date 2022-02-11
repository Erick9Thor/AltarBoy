#include "ResourceMaterial.h"
#include "Application.h"

Hachiko::ResourceMaterial::ResourceMaterial(UID uid) :
    Resource(uid, Type::MATERIAL) {}

Hachiko::ResourceMaterial::~ResourceMaterial()
{
    App->texture->Unload(diffuse);
    App->texture->Unload(specular);
}
