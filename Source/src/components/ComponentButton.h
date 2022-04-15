#pragma once

#include "Component.h"
#include "Globals.h"
#include "ISelectable.h"

namespace Hachiko
{
    class GameObject;

    class ComponentTransform2D;

    class ComponentButton : public Component, public ISelectable
    {
    public:
        ComponentButton(GameObject* container);
        ~ComponentButton() override = default;
        
        void Activate() override {}

        static Type GetType();

        void DrawGui() override;

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;
    };
    
    inline Component::Type Hachiko::ComponentButton::GetType()
    {
        return Type::BUTTON;
    }
} // namespace Hachiko
