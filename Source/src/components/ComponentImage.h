#pragma once

#include "Component.h"
#include "Globals.h"

namespace Hachiko
{
    class GameObject;

    class ComponentTransform2D;
    class Program;

    class ComponentImage : public Component
    {
    public:
        ComponentImage(GameObject* container);
        ~ComponentImage() override = default;

        static Type GetType();

        void DrawGui() override;
        void Draw(ComponentTransform2D* transform, Program* program) const;
    };
} // namespace Hachiko

inline Hachiko::Component::Type Hachiko::ComponentImage::GetType()
{
    return Type::IMAGE;
}
