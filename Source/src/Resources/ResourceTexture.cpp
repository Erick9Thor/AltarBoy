#include "Core/hepch.h"
#include "ResourceTexture.h"

Hachiko::ResourceTexture::ResourceTexture(UID id)
	: Resource(id, Type::TEXTURE)
{}

Hachiko::ResourceTexture::~ResourceTexture() 
{
    glDeleteTextures(1, &id);
}
