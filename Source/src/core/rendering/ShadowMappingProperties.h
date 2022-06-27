#pragma once

namespace Hachiko::ShadowMappingDefaults
{
    constexpr const float GAUSSIAN_FILTER_BLUR_AMOUNT = 0.62f;
    constexpr const float MIN_VARIANCE = 0.001f;
    constexpr const float LIGHT_BLEEDING_REDUCTION_AMOUNT = 0.001f;
    constexpr const float BIAS = 0.00001f;
    constexpr const float EXPONENT = 14.37f;
} // namespace Hachiko::ShadowMappingDefaults

namespace Hachiko
{

class Program;

class ShadowMappingProperties
{
public:
    ShadowMappingProperties();
    ~ShadowMappingProperties() = default;

    void BindForShadowMapGenerationPass(const Program* program) const;
    void BindForLightingPass(const Program* program) const;

    void SetGaussianFilterBlurAmount(float value);
    void SetMinVariance(float value);
    void SetLightBleedingReductionAmount(float value);
    void SetBias(float value);
    void SetExponent(float value);
    
    [[nodiscard]] float GetGaussianFilterBlurAmount() const;
    [[nodiscard]] float GetMinVariance() const;
    [[nodiscard]] float GetLightBleedingReductionAmount() const;
    [[nodiscard]] float GetBias() const;
    [[nodiscard]] float GetExponent() const;

private:
    void BindCommon(const Program* program) const;

private:
    float gaussian_filter_blur_amount = 0.0f;
    float min_variance = 0.0f;
    float light_bleeding_reduction_amount = 0.0f;
    float bias = 0.0f;
    float exponent = 0.0f;
};

} // namespace Hachiko
