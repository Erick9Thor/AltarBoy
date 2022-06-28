#pragma once


namespace Hachiko
{
class ComponentDirLight;
class ComponentCamera;
class Program;

class ShadowManager
{

public:
    ShadowManager();
    ~ShadowManager();

    void GenerateShadowMap();
    void FreeShadowMap();
    void BindShadowMapTexture(unsigned location) const; 
    void CalculateLightFrustum();
    void BindShadowMapGenerationPassUniforms(const Program* program) const;
    void BindLightingPassUniforms(const Program* program) const;
    void ApplyGaussianBlur(const Program* program) const;
    void BindBufferForDrawing() const;
    void UnbindBuffer() const;

private:
    const ComponentDirLight* GetDirectionalLight() const;
    void BindCommonUniforms(const Program* program) const;
    bool DetectCameraChanges();
    bool DetectLightChanges();

private:
    float4x4 _directional_light_projection = float4x4::identity;
    float4x4 _directional_light_view = float4x4::identity;
    float3 _directional_light_direction = float3::zero;
    float4x4 _camera_transform = float4x4::identity;
    float4x4 _camera_projection = float4x4::identity;
    float4x4 _camera_view = float4x4::identity;
    bool _camera_exists = false;
    bool _light_exists = false;
    unsigned _shadow_map_fbo = 0;
    unsigned _shadow_map_texture = 0;
    unsigned _shadow_map_depth = 0;
    unsigned _shadow_map_fbo_temp = 0;
    unsigned _shadow_map_texture_temp = 0;
    const unsigned _shadow_map_width = 4096;
    const unsigned _shadow_map_height = 4096;
};

} // namespace Hachiko