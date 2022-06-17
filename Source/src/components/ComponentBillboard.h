#pragma once

#include "Component.h"

class ImGradient;
struct ImGradientMark;

namespace Hachiko
{
    enum class BillboardType
    {
        NORMAL,
        HORIZONTAL,
        VERTICAL
    };

    enum class BillboardRenderMode
    {
        B_ADDITIVE,
        B_TRANSPARENT
    };

    class ComponentBillboard : public Component
    {
    public:
        ComponentBillboard(GameObject* container);
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
        bool in_scene = false;
        bool is_playing = false;
        bool play_on_awake = false;
        int frame_counter = 0;
        int skip_frames = 0;
        float time = 0.0f;
        float billboard_lifetime = 5.0f;
        float blend_factor = 0.0f;
        BillboardRenderMode render_mode = BillboardRenderMode::B_ADDITIVE;
        
        // Orientation
        bool is_horizontal = false;
        float4x4 model_stretch = float4x4::identity;
        float3 direction = float3::zero;
        BillboardType type = BillboardType::HORIZONTAL;

        // Texture
        bool flip_texture[2] = {false, false};
        UID textureID = 0;
        ResourceTexture* texture = nullptr;
        
        // Animation
        bool animation_loop = true;
        bool play_animation = false;
        bool has_flip_x = false;
        bool has_flip_y = false;
        int x_tiles = 1;
        int y_tiles = 1;
        int has_texture = 0;
        float x_factor = 1.0f;
        float y_factor = 1.0f;
        float current_frame = 0.0f;
        float2 animation_index = {0.0f, 0.0f};

        // Color gradient
        bool has_color_gradient = false;
        bool play_color_gradient = false;
        bool color_loop = true;
        int color_cycles = 1.0f;
        float color_frame = 0.0f;
        ImGradient* gradient = nullptr;
        ImGradientMark* dragging_gradient = nullptr;
        ImGradientMark* selected_gradient = nullptr;

    private:
        void Reset();
        void AddTexture();
        void RemoveTexture();
        void UpdateAnimationData();
        void UpdateColorOverLifetime();
        void PublishIntoScene();
        void DetachFromScene();
        void GetOrientationMatrix(ComponentCamera* camera, float4x4& model_matrix);
    };
}
