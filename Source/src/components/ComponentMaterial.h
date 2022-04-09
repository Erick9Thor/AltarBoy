#pragma once

#include "Component.h"

namespace Hachiko
{
    class ResourceMaterial;

    class ComponentMaterial : public Component
    {
    public:
        ComponentMaterial(GameObject* container);
        ~ComponentMaterial() override;

        static Type GetType()
        {
            return Type::MATERIAL;
        }

        [[nodiscard]] const ResourceMaterial* GetMaterial() const
        {
            return material;
        }

        void SetResourceMaterial(ResourceMaterial* res)
        {
            delete material;
            material = res;
        }

        [[nodiscard]] const std::string& GetModelName() const
        {
            return model_name;
        }

        void SetModelName(const std::string& name)
        {
            model_name = name;
        }

        void DrawGui() override;
        void LoadMaterial(const std::string& material_path);

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;
        
        char diffuse_filename_buffer[MAX_PATH] = "Diffuse Filename\0";
        char specular_filename_buffer[MAX_PATH] = "Specular Filename\0";

        bool use_diffuse_texture = false;
        bool use_specular_texture = false;

    private:
        ResourceMaterial* material{};
        std::string model_name;
    };
}
