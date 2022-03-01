#pragma once

namespace Hachiko
{
    class ComponentMesh;
    class ComponentMaterial;

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

        struct Material
        {
            float4 diffuseColor;
            float4 specularColor;
            float shininess; // float smoothness;
            int hasDiffuseMap;
            int hasSpecularMap;
            int hasNormalMap;
            TexAddress diffuseMap;
            TexAddress specularMap;
            TexAddress normalMap;
            int padding0;
            int padding1;
        };

        TextureBatch();
        ~TextureBatch();

        void AddTexture(const Texture* texture);
        void AddMaterial(const ComponentMaterial* material);

        void GenerateBatch();

        void GenerateMaterials(const std::vector<const ComponentMesh*>& components);

        void UpdateTextureBatch();
        void UpdateMaterials(unsigned ssbo_id);

        void ImGuiWindow(); // Debug window

        bool EqualLayout(const TextureArray& texuteLayout, const Texture& texture);

        // TODO: make private
        // TODO: consider creating a ResourceTexture

        std::map<const Texture*, TexAddress*> resources; // contains all the Texture resources and their address
        std::vector<TextureArray*> textureArrays; // contains all the texture arrays

        std::vector<Material> materials;

        bool loaded = false;
    };
} // namespace Hachiko