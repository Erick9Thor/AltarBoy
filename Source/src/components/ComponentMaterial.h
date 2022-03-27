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
            delete material;
            material = res;
        }

        
        [[nodiscard]] const std::string& GetResourcePath() const
        {
            return asset_path;
        }

        void SetResourcePath(const std::string& path)
        {
            asset_path = path;
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
        void LoadMaterial(UID mesh_id);

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;

    private:
        ResourceMaterial* material{};
        std::string asset_path;
        std::string model_name;
    };
}
