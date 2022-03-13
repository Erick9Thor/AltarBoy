#include "Core/hepch.h"
#include "ResourceTexture.h"

Hachiko::ResourceTexture::ResourceTexture(UID id)
	: Resource(id, Type::TEXTURE)
{}

Hachiko::ResourceTexture::~ResourceTexture() 
{
    glDeleteTextures(1, &id);
    delete[] data;
}

void Hachiko::ResourceTexture::GenerateBuffer() 
{
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

    glTexImage2D(GL_TEXTURE_2D, 0, bpp, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}
