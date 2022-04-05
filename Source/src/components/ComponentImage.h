#pragma once

#include "Component.h"
#include "Globals.h"

#include "modules/ModuleTexture.h"

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

        void Save(JsonFormatterValue j_component) const override;
        void Load(JsonFormatterValue j_component) override;

        void Import(const char* path);


        Texture image;
        Texture hover_image;

        float4 color = float4::one;
        bool use_image = false;
    };
} // namespace Hachiko

inline Hachiko::Component::Type Hachiko::ComponentImage::GetType()
{
    return Type::IMAGE;
}
