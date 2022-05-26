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
        
        void Start() override;
        void Update() override;
        void Play();
        void Stop() override;

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;
        
        void DrawGui() override;
        void Draw(ComponentCamera* camera, Program* program) override;

    private:
        // General
        bool started = false;
        bool is_playing = false;
        bool play_on_awake = false;
        float time = 0.0f;
        float current_frame = 0.0f;
        float billboard_lifetime = 5.0f;
        BillboardRenderMode render_mode = BillboardRenderMode::B_ADDITIVE;
        
        // Orientation
        bool is_horizontal = false;
        bool flip_texture[2] = {false, false};
        float4x4 model_stretch = float4x4::identity;
        float3 initPos = float3::zero;
        float3 previousPos = float3::zero;
        float3 direction = float3::zero;
        BillboardType type = BillboardType::HORIZONTAL;

        // Texture
        UID textureID = 0;
        ResourceTexture* texture = nullptr;
        float3 textureIntensity = {1.0f, 1.0f, 1.0f};
        
        // Animation
        bool animation_loop = true;
        bool flip_x = false;
        bool flip_y = false;
        int x_tiles = 1;
        int y_tiles = 1;
        int has_diffuse_map = 0;
        float x_factor = 1.0f;
        float y_factor = 1.0f;
        float2 animation_index = {0.0f, 0.0f};
        
        // Color gradient
        bool has_color_gradient = false;
        bool color_loop = true;
        int color_cycles = 1.0f;
        float color_frame = 0.0f;
        ImGradient* gradient = nullptr;
        ImGradientMark* draggingGradient = nullptr;
        ImGradientMark* selectedGradient = nullptr;

    private:
        void Reset();
        void AddTexture();
        void RemoveTexture();
        void UpdateAnimationIndex();
        void UpdateColorOverLifetime();
        void GetOrientationMatrix(ComponentCamera* camera, float4x4& model_matrix);
    };
}
