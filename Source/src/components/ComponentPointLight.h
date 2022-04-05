#pragma once

#include "components/Component.h"
#include "HachikoApiDefine.h"

namespace Hachiko
{
    class HACHIKO_API ComponentPointLight : public Component
    {
    public:
        // TODO: add more light types
        ComponentPointLight(GameObject* conatiner);
        ~ComponentPointLight() override;

        static Type GetType()
        {
            return Type::POINTLIGHT;
        }

        void DebugDraw() override;

        [[nodiscard]] bool IsActive() const
        {
            return active;
        }

        void SetActive(bool is_active)
        {
            active = is_active;
        }

        float3 GetPosition() const;

        void Save(JsonFormatterValue j_component) const override;
        void Load(JsonFormatterValue j_component) override;

        void DrawGui() override;

        float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
        float intensity = 1.0f;
        float radius = 250.0f;

    private:
        bool active = true;
        bool draw_sphere = false;
    };
}
