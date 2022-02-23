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

    unsigned index = 0;
    for (TextureArray* textureArray : textureArrays)
    {
        glGenTextures(1, &textureArray->id);
        glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray->id);
        // TODO: INTRODUCE MIPMAPS
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, textureArray->format, textureArray->width, textureArray->height, textureArray->depth);
        unsigned depth = 0;
        for (auto& resource : resources)
        {
            if (EqualLayout(*textureArray, *resource.first))
            {
                resource.second->texIndex = textureArray->id;
                resource.second->layerIndex = depth;

                // TODO extract type and texture_data from Texture
                //glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, depth, textureArray->width, textureArray->height, 1, textureArray->format, type, texture_data);
                ++depth;
            }
        }
    }
}

bool Hachiko::TextureBatch::EqualLayout(const TextureArray& texuteLayout, const Texture& texture)
{
    return (texuteLayout.width == texture.width && texuteLayout.height == texture.height && texuteLayout.format == texture.format);
}