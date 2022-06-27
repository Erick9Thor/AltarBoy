#include "core/hepch.h"
#include "core/rendering/ShadowMappingProperties.h"
#include "core/rendering/Uniforms.h"
#include "core/rendering/Program.h"

Hachiko::ShadowMappingProperties::ShadowMappingProperties() 
    : gaussian_filter_blur_amount(ShadowMappingDefaults::GAUSSIAN_FILTER_BLUR_AMOUNT)
    , min_variance(ShadowMappingDefaults::MIN_VARIANCE)
    , light_bleeding_reduction_amount(ShadowMappingDefaults::LIGHT_BLEEDING_REDUCTION_AMOUNT)
    , bias(ShadowMappingDefaults::BIAS)
    , exponent(ShadowMappingDefaults::EXPONENT)
{
}

void Hachiko::ShadowMappingProperties::BindForShadowMapGenerationPass(const Program* program) const 
{
    BindCommon(program);
}

void Hachiko::ShadowMappingProperties::BindForLightingPass(const Program* program) const 
{
    BindCommon(program);

    program->BindUniformFloat(
        Uniforms::ShadowMap::LIGHT_BLEEDING_REDUCTION_AMOUNT, 
        &light_bleeding_reduction_amount);
    program->BindUniformFloat(Uniforms::ShadowMap::MIN_VARIANCE, &min_variance);
    program->BindUniformFloat(Uniforms::ShadowMap::BIAS, &bias);
}

void Hachiko::ShadowMappingProperties::SetGaussianFilterBlurAmount(float value) 
{
    gaussian_filter_blur_amount = value;

    std::clamp<float>(gaussian_filter_blur_amount, 0.0f, FLT_MAX);
}

void Hachiko::ShadowMappingProperties::SetMinVariance(float value) 
{
    min_variance = value;

    std::clamp<float>(min_variance, 0.0f, 1.0f);
}

void Hachiko::ShadowMappingProperties::SetLightBleedingReductionAmount(float value) 
{
    light_bleeding_reduction_amount = value;

    std::clamp<float>(light_bleeding_reduction_amount, 0.0f, 1.0f);
}

void Hachiko::ShadowMappingProperties::SetBias(float value) 
{
    bias = value;

    std::clamp<float>(bias, 0.0f, 1.0f);
}

void Hachiko::ShadowMappingProperties::SetExponent(float value) 
{
    exponent = value;

    std::clamp<float>(exponent, 0.0f, FLT_MAX);
}

float Hachiko::ShadowMappingProperties::GetGaussianFilterBlurAmount() const
{
    return gaussian_filter_blur_amount;
}

float Hachiko::ShadowMappingProperties::GetMinVariance() const
{
    return min_variance;
}

float Hachiko::ShadowMappingProperties::GetLightBleedingReductionAmount() const
{
    return light_bleeding_reduction_amount;
}

float Hachiko::ShadowMappingProperties::GetBias() const
{
    return bias;
}

float Hachiko::ShadowMappingProperties::GetExponent() const
{
    return exponent;
}

void Hachiko::ShadowMappingProperties::BindCommon(const Program* program) const 
{
    program->BindUniformFloat(Uniforms::ShadowMap::EXPONENT, &exponent);
}
