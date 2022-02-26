#pragma once

namespace Hachiko
{
    class TextureBatch
    {
    public:
        struct TexAddress
        {
            unsigned texIndex = 0; // contains the array where is located
            int layerIndex = 0; // contains the depth in the array where is located
        };

        struct TextureArray
        {
            unsigned id = 0;
            unsigned depth = 0;
            unsigned width = 0;
            unsigned height = 0;
            unsigned format = 0;
        };

        TextureBatch();
        ~TextureBatch();

        void AddTexture(const Texture* texture);

        void GenerateBatch();

        void Bind();

        bool EqualLayout(const TextureArray& texuteLayout, const Texture& texture);

        // TODO: make private
        // TODO: consider creating a ResourceTexture

        std::map<const Texture*, TexAddress*> resources; // contains all the Texture resources and their address
        std::vector<TextureArray*> textureArrays; // contains all the texture arrays

        bool loaded = false;
    };
} // namespace Hachiko