#include "core/hepch.h"
#include "TextureBatch.h"

Hachiko::TextureBatch::TextureBatch() 
{
}

Hachiko::TextureBatch::~TextureBatch() 
{
    for (auto& resource : resources)
    {
        delete resource.second;
    }
    resources.clear();

    for (TextureArray* textureArray : textureArrays)
    {
        glDeleteTextures(textureArray->depth, &textureArray->id);
        delete textureArray;
    }
    textureArrays.clear();
}

void Hachiko::TextureBatch::AddTexture(const Texture* texture) 
{
    auto it = resources.find(texture);

    if (it == resources.end())
    {
        // It will be formatted when generating batches
        resources[texture] = new TexAddress();

        // Search for the its textureArray
        bool textureArrayFound = false;
        for (TextureArray* textureArray : textureArrays)
        {
            if (EqualLayout(*textureArray, *texture))
            {
                textureArrayFound = true;
                textureArray->depth += 1;
                break;
            }
        }

        // If there is none, create it
        if (!textureArrayFound)
        {
            TextureArray* textureArray = new TextureArray();
            textureArray->depth = 1;
            textureArray->width = texture->width;
            textureArray->height = texture->height;
            textureArray->format = texture->format;

            textureArrays.push_back(textureArray);
        }

        // Set the flag as not all textures are loaded
        loaded = false;
    }
}

void Hachiko::TextureBatch::GenerateBatch() 
{
    // TODO: improve performance (for inside for)

    int maxLayers; // maximum number of array texture layers 
    int maxUnits; // maximum number of texture units
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayers);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits);

    // TODO: security checks with these maximums

    unsigned index = 0;
    for (TextureArray* textureArray : textureArrays)
    {
        glGenTextures(1, &textureArray->id);
        glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray->id);

        // Array texture parameters
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 2);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Generate texture array
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 3, textureArray->format, textureArray->width, textureArray->height, textureArray->depth);
        unsigned depth = 0;
        for (auto& resource : resources)
        {
            if (EqualLayout(*textureArray, *resource.first))
            {
                resource.second->texIndex = textureArray->id;
                resource.second->layerIndex = depth;

                byte* new_array = new byte[resource.first->width * resource.first->height * resource.first->bpp];

                glGetTexImage(GL_TEXTURE_2D, 0, textureArray->format, GL_UNSIGNED_BYTE, (void*)(new_array));

                glTexSubImage3D(
                    GL_TEXTURE_2D_ARRAY, // target
                    0, // level
                    0, // xoffset
                    0, // yoffset
                    depth, // zoffset
                    textureArray->width, // width
                    textureArray->height, // height
                    1, // depth
                    textureArray->format, // format
                    GL_UNSIGNED_BYTE, // type
                    new_array // texture_data
                );

                delete[] new_array;
                ++depth;
            }
        }
    }
}

void Hachiko::TextureBatch::Bind() 
{
    // TODO: binding
}

bool Hachiko::TextureBatch::EqualLayout(const TextureArray& texuteLayout, const Texture& texture)
{
    return (texuteLayout.width == texture.width && texuteLayout.height == texture.height && texuteLayout.format == texture.format);
}