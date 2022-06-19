#pragma once

#include "Component.h"

class ImGradient;
struct ImGradientMark;

namespace Hachiko
{
    class HACHIKO_API ComponentBillboard : public Component
    {
    public:
        ComponentBillboard(GameObject* container);
        ~ComponentBillboard() override;
        
        void Start() override;
        void Update() override;

        void Play();
        void Pause();
        void Restart();
        void Stop() override;

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;
        
        void DrawGui() override;
        void Draw(ComponentCamera* camera, Program* program) override;

    private:
        bool in_scene = false;
        ParticleSystem::Emitter::State state = ParticleSystem::Emitter::State::STOPPED;
        float elapsed_time = 0.0f;

        // Sections
        bool parameters_section = true;
        bool renderer_section = true;
        bool texture_section = true;
        bool animation_section = false;
        bool color_section = false;
        
        // Parameters
        bool loop = false;
        bool play_on_awake = false;
        float duration = 5.0f;
        ParticleSystem::VariableTypeProperty start_delay {float2::zero, 1.0f, false, true};
        ParticleSystem::VariableTypeProperty start_size {float2::one, 1.0f};
        ParticleSystem::VariableTypeProperty start_rotation {float2::zero, 1.0f};

        // Render
        ParticleSystem::ParticleProperties properties;

        // Texture
        bool2 flip_texture = bool2::False;
        float2 tiles = float2::one;
        float2 factor = float2::one;
        ResourceTexture* texture = nullptr;

        // Animation
        float animation_speed = 0.0f;
        float current_frame = 0.0f;
        float2 animation_index = {0.0f, 0.0f};
        float blend_factor = 0.0f;
        float total_tiles = 1.0f;

        // Color
        int color_cycles = 1.0f;
        float color_frame = 0.0f;
        ImGradient* gradient = nullptr;
        ImGradientMark* dragging_gradient = nullptr;
        ImGradientMark* selected_gradient = nullptr;

        // General
        int frame_counter = 0;
        float time = 0.0f;
        
        // Orientation
        bool is_horizontal = false;
        float4x4 model_stretch = float4x4::identity;
        float3 direction = float3::zero;
        void Reset();
        void PublishIntoScene();
        void DetachFromScene();

        inline bool HasTexture();
        void AddTexture();
        void RemoveTexture();
        
        void UpdateAnimationData();
        void UpdateColorOverLifetime();

        void GetOrientationMatrix(ComponentCamera* camera, float4x4& model_matrix);
        void DisplayControls();

    };
}
