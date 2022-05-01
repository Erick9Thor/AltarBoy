#pragma once

#include "components/Component.h"
#include "resources/ResourceMaterial.h"

namespace Hachiko
{
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
            material = res;
            SetID(material->GetID());
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
        void LoadMaterial(UID id);

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;

        void ChangeMaterial();
        
    private:
        ResourceMaterial* material{};
        std::string model_name;
    };
}
