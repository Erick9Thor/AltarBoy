#pragma once

#include "Component.h"

class ImGradient;
struct ImGradientMark;

namespace Hachiko
{
    enum class BillboardType
    {
        NORMAL,
        STRETCH,
        HORIZONTAL,
        VERTICAL
    };

    enum class RenderMode
    {
        B_ADDITIVE,
        B_TRANSPARENT
    };

    class ComponentBillboard : public Component
    {
    public:
        ComponentBillboard(GameObject* container, UID id = 0);
        ~ComponentBillboard() override = default;

        void Save(YAML::Node& node) const override {};
        void Load(const YAML::Node& node) override {};
        
        void DrawGui() override {};

    private:
        float4x4 modelStretch = float4x4::identity;
        float3 initPos = float3::zero;
        float3 previousPos = float3::zero;
        float3 direction = float3::zero;

        bool isStarted = false;
        bool isPlaying = false;
        float time = 0.0f;
        float currentFrame = 0.0f;
        float colorFrame = 0.0f;

        // General
        float billboardLifetime = 5.0f;
        bool playOnAwake = false;

        // Render
        BillboardType billboardType = BillboardType::NORMAL;
        RenderMode renderMode = RenderMode::B_ADDITIVE;
        UID textureID = 0; // ID of the image
        float3 textureIntensity = {1.0f, 1.0f, 1.0f};

        bool isHorizontalOrientation = false;
        bool flipTexture[2] = {false, false};

        // Texture Sheet Animation
        unsigned Xtiles = 1;
        unsigned Ytiles = 1;
        float animationCycles = 1.0f;
        bool animationLoop = true;

        // Color over Lifetime
        bool colorOverLifetime = false;
        float colorCycles = 1.0f;
        bool colorLoop = true;
        ImGradient* gradient = nullptr;
        ImGradientMark* draggingGradient = nullptr;
        ImGradientMark* selectedGradient = nullptr;
    };
}
