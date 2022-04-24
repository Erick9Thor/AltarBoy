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
            int texIndex = 0; // contains the array where is located
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
            float4 diffuse_color;
            float4 specular_color;
            float shininess; // float smoothness;
            int has_diffuse_map;
            int has_specular_map;
            int has_normal_map;
            TexAddress diffuse_map;
            TexAddress specular_map;
            TexAddress normal_map;
            float2 padding = float2::zero;
        };

        TextureBatch();
        ~TextureBatch();

        void AddMaterial(const ComponentMaterial* material);
        void AddTexture(const ResourceTexture* texture);

        void GenerateBatch();

        void Draw(const std::vector<const ComponentMesh*>& components);

        void GenerateMaterials(const std::vector<const ComponentMesh*>& components);

        void UpdateTextureBatch();
        void UpdateMaterials();

        void ImGuiWindow(); // Debug window

        bool EqualLayout(const TextureArray& texuteLayout, const ResourceTexture& texture);

        // TODO: make private
        // TODO: consider creating a ResourceTexture

        std::map<const ResourceTexture*, TexAddress*> resources; // contains all the Texture resources and their address
        std::vector<TextureArray*> texture_arrays; // contains all the texture arrays

        std::vector<Material> materials;

        bool loaded = false;
    };
} // namespace Hachiko