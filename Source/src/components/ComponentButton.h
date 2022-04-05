#pragma once

#include "components/Component.h"
#include "Globals.h"
#include "components/ISelectable.h"

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

        void Save(JsonFormatterValue j_component) const override;
        void Load(JsonFormatterValue j_component) override;
    };
    
    inline Component::Type Hachiko::ComponentButton::GetType()
    {
        return Type::BUTTON;
    }
} // namespace Hachiko
