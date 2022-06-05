#pragma once

#include "components/Component.h"
#include "Globals.h"

#include "modules/ModuleTexture.h"

namespace Hachiko
{
    class GameObject;

    class ComponentTransform2D;
    class Program;

    class HACHIKO_API ComponentImage : public Component
    {
    public:
        ComponentImage(GameObject* container);
        ~ComponentImage() override = default;

        void DrawGui() override;
        void Draw(ComponentTransform2D* transform, Program* program) const;

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;

    private:
        void LoadImageResource(UID image_uid, bool is_hover = false);

        ResourceTexture* image = nullptr;
        ResourceTexture* hover_image = nullptr;

        float4 color = float4::one;
        float4 hover_color = float4::one;
        bool use_image = false;
        bool use_hover_image = false;
    };
} // namespace Hachiko
