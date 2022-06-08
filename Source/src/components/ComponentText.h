#pragma once

#include "Component.h"
#include "Globals.h"

#include "resources/ResourceFont.h"
#include "FTLabel.h"

namespace Hachiko
{
    class GameObject;

    class ComponentTransform2D;
    class Program;

    class HACHIKO_API ComponentText : public Component
    {
        friend class ComponentTransform2D;
    public:
        ComponentText(GameObject* container);
        ~ComponentText() override;

        void DrawGui() override;
        void Draw(ComponentTransform2D* transform, Program* program);

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;

        void SetText(const char* new_text);
        void SetFontSize(int new_size);
        void SetFontColor(const float4& new_color);

    private:
        void Invalidate()
        {
            dirty = true;
        }
        void LoadFont(UID id);
        void RefreshLabel(ComponentTransform2D* transform);
        void BuildLabel(ComponentTransform2D* transform);

        bool dirty = true;

        ResourceFont* font = nullptr;
        std::unique_ptr<FTLabel> label = nullptr;

        std::string label_text = "Sample Text";


        float4 font_color = float4::one;
        int font_size = 28;

        char font_filename_buffer[MAX_PATH] = "Image Filename\0";
    };
} // namespace Hachiko
