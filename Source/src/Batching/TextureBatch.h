#pragma once

namespace Hachiko
{
    class ComponentMeshRenderer;
    class ResourceMaterial;

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
            unsigned diffuse_flag;
            unsigned specular_flag;
            unsigned normal_flag;
            unsigned metallic_flag;
            TexAddress diffuse_map;
            TexAddress specular_map;
            TexAddress normal_map;
            TexAddress metallic_map;
            float smoothness;
            float metalness_value;
            unsigned is_metallic;
            unsigned smoothness_alpha;
            unsigned is_transparent;
            unsigned padding[3];
        };

        TextureBatch();
        ~TextureBatch();

        void AddMaterial(const ResourceMaterial* material);
        void AddTexture(const ResourceTexture* texture);

        void GenerateBatch(unsigned component_count);

        void Draw(const std::vector<const ComponentMeshRenderer*>& components, bool use_first_segment, unsigned component_count);

        void GenerateMaterials(const std::vector<const ComponentMeshRenderer*>& components);

        void BindBuffers(bool use_first_segment, int component_count);
        void UpdateTextureBatch();
        void UpdateMaterials(const std::vector<const ComponentMeshRenderer*>& components, bool use_first_segment, unsigned component_count);

        void ImGuiWindow(); // Debug window

        bool EqualLayout(const TextureArray& texuteLayout, const ResourceTexture& texture);

        // TODO: make private
        // TODO: consider creating a ResourceTexture

        std::map<const ResourceTexture*, TexAddress*> resources; // contains all the Texture resources and their address
        std::vector<TextureArray*> texture_arrays; // contains all the texture arrays

        std::vector<Material> materials;

        unsigned material_buffer;

        Material* material_buffer_data = nullptr;


        bool loaded = false;
    };
} // namespace Hachiko