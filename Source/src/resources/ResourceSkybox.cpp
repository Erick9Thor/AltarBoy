#include "core/hepch.h"

#include "ResourceSkybox.h"

Hachiko::ResourceSkybox::ResourceSkybox() : Resource(Resource::Type::SKYBOX) {}

Hachiko::ResourceSkybox::ResourceSkybox(UID id) : Resource(id, Resource::Type::SKYBOX) {}

Hachiko::ResourceSkybox::~ResourceSkybox()
{
    if (id != 0)
    {
        glDeleteTextures(1, &id);

    }
    delete[] data;
}

void Hachiko::ResourceSkybox::GenerateBuffer()
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

    glTexImage2D(GL_TEXTURE_2D, 0, bpp, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}