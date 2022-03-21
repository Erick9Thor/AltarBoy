#pragma once

#include "Component.h"
#include "resources/ResourceMaterial.h"

#include "assimp/scene.h"

namespace Hachiko
{
    class ComponentMaterial : public Component
    {
    public:
        ComponentMaterial(GameObject* conatiner);
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
            material = res;
        }

        void DrawGui() override;

        void Import(aiMaterial* assimp_material, const std::string& model_path, const std::string& model_name);
        void Save(YAML::Node& node) const override {};
        void Load(const YAML::Node& node) override {};

        bool use_diffuse_texture = false;
        bool use_specular_texture = false;

    private:
        ResourceMaterial* material{};
    };
}
