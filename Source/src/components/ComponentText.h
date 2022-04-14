#pragma once

#include "Component.h"
#include "Globals.h"

#include "modules/ModuleTexture.h"
#include "FTLabel.h"

namespace Hachiko
{
    class GameObject;

    class ComponentTransform2D;
    class Program;

    class ComponentText : public Component
    {
    public:
        ComponentText(GameObject* container);
        ~ComponentText() override = default;

        static Type GetType();

        void DrawGui() override;
        void Draw(ComponentTransform2D* transform, Program* program);

        void Save(JsonFormatterValue j_component) const override;
        void Load(JsonFormatterValue j_component) override;

        

        void Invalidate()
        {
            dirty = true;
        }

    private:
        void RefreshLabel(ComponentTransform2D* transform);
        
        bool dirty = true;
        void BuildLabel(ComponentTransform2D* transform);


        Font font;
        std::unique_ptr<FTLabel> label = nullptr;

        std::string label_text = "Sample Text";

        float4 font_color = float4::one;
        float font_size = 28.f;

        char font_filename_buffer[MAX_PATH] = "Image Filename\0";
    };
} // namespace Hachiko

inline Hachiko::Component::Type Hachiko::ComponentText::GetType()
{
    return Type::TEXT;
}
