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

    // TODO: Send this to ComponentMeshRenderer or ResourceMesh
    enum class BillboardRenderMode
    {
        B_ADDITIVE,
        B_TRANSPARENT
    };

    class ComponentBillboard : public Component
    {
    public:
        ComponentBillboard(GameObject* container, UID id = 0);
        ~ComponentBillboard() override;
        
        void Update() override;

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;
        
        void DrawGui() override;
        void Draw(ComponentCamera* camera, Program* program) override;

    private:
        const ImVec4 textColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
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
        BillboardType billboardType = BillboardType::HORIZONTAL;
        BillboardRenderMode renderMode = BillboardRenderMode::B_ADDITIVE;
        UID textureID = 0; // ID of the image
        float3 textureIntensity = {1.0f, 1.0f, 1.0f};
        ResourceTexture* texture = nullptr;

        bool isHorizontalOrientation = false;
        bool flipTexture[2] = {false, false};

        // Texture Sheet Animation
        int Xtiles = 1;
        int Ytiles = 1;
        float animationCycles = 1.0f;
        bool animationLoop = true;

        // Color over Lifetime
        bool colorOverLifetime = false;
        float colorCycles = 1.0f;
        bool colorLoop = true;
        ImGradient* gradient = nullptr;
        ImGradientMark* draggingGradient = nullptr;
        ImGradientMark* selectedGradient = nullptr;

        void AddTexture();
        void RemoveTexture();
    };
}
