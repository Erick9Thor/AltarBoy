#pragma once

#include "Component.h"
#include "Globals.h"

namespace Hachiko
{
    class GameObject;

    class ComponentCanvas;
    class Program;

    class ComponentCanvasRenderer : public Component
    {
    public:
        ComponentCanvasRenderer(GameObject* container);
        ~ComponentCanvasRenderer() override = default;

        static Type GetType();

        void DrawGui() override;

        bool HasDependentComponents(GameObject* game_object) const override;

        ComponentCanvas* FindClosestParentCanvas() const;
        void Render(Program* program) const;
    };
} // namespace Hachiko

inline Hachiko::Component::Type Hachiko::ComponentCanvasRenderer::GetType()
{
    return Type::CANVAS_RENDERER;
}