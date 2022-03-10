#pragma once

#include "Component.h"
#include "Globals.h"

namespace Hachiko
{
    class GameObject;

    class ComponentTransform2D;

    class ComponentButton : public Component
    {
    public:
        ComponentButton(GameObject* container);
        ~ComponentButton() override = default;

        static Type GetType();

        void DrawGui() override;
    };
} // namespace Hachiko

inline Hachiko::Component::Type Hachiko::ComponentButton::GetType()
{
    return Type::BUTTON;
}
