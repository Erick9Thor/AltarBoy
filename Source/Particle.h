
namespace Hachiko
{
    enum class ParticleRenderMode
    {
        ADDITIVE_PARTICLE,
        TRANSPARENT_PARTICLE
    };

    class Particle
    {
    public:
        Particle() = default;
        virtual ~Particle() = default;

        void Activate();
        void Deactivate();

        // Rendering
        void Draw(ComponentCamera* camera, Program* program);

        // Settings


    private:
        // General
        bool is_playing = false;
        bool play_on_awake = false;
        float time = 0.0f;
        float lifetime = 5.0f;
        ParticleRenderMode render_mode = ParticleRenderMode::ADDITIVE_PARTICLE;

        //// Orientation
        //bool is_horizontal = false;
        //float4x4 model_stretch = float4x4::identity;
        //float3 initPos = float3::zero;
        //float3 previousPos = float3::zero;
        //float3 direction = float3::zero;
        //BillboardType type = BillboardType::HORIZONTAL;

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

        void UpdateColorOverLifetime();
        void GetOrientationMatrix(ComponentCamera* camera, float4x4& model_matrix);
    };
}