#pragma once

class ImGradient;
struct ImGradientMark;

namespace Hachiko
{
    enum class ParticleOrientationType
    {
        NORMAL,
        STRETCH,
        HORIZONTAL,
        VERTICAL
    };

    enum class ParticleRenderMode
    {
        PARTICLE_ADDITIVE,
        PARTICLE_TRANSPARENT
    };

    class Particle
    {
    public:
        Particle();
        virtual ~Particle();

        void Start();
        void Update();
        void Play();
        void Stop();

        void Save(YAML::Node& node) const;
        void Load(const YAML::Node& node);

        void DrawGui();
        void Draw(ComponentCamera* camera, Program* program);

        // Getters and Setters instead of DrawGui
    private:
        // General
        bool is_playing = false;
        bool play_on_awake = false;
        int frame_counter = 0;
        int skip_frames = 0;
        float time = 0.0f;
        float billboard_lifetime = 5.0f;
        ParticleRenderMode render_mode = ParticleRenderMode::PARTICLE_ADDITIVE;

        // Orientation
        bool is_horizontal = false;
        float4x4 model_stretch = float4x4::identity;
        float3 initPos = float3::zero;
        float3 previousPos = float3::zero;
        float3 direction = float3::zero;
        ParticleOrientationType type = ParticleOrientationType::HORIZONTAL;

        // Texture
        bool flip_texture[2] = {false, false};
        UID textureID = 0;
        ResourceTexture* texture = nullptr;
        float3 textureIntensity = {1.0f, 1.0f, 1.0f}; // TODO: Not used yet

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
        ImGradientMark* draggingGradient = nullptr;
        ImGradientMark* selectedGradient = nullptr;

    private:
        void Reset();
        void AddTexture();
        void RemoveTexture();
        void UpdateAnimationData();
        void UpdateColorOverLifetime();
        void GetOrientationMatrix(ComponentCamera* camera, float4x4& model_matrix);
    };
} // namespace Hachiko
