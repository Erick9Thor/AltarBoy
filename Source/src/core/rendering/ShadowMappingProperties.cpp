#include "core/hepch.h"
#include "core/rendering/ShadowMappingProperties.h"
#include "core/rendering/Uniforms.h"
#include "core/rendering/Program.h"
#include "ui/widgets/Widgets.h"

Hachiko::ShadowMappingProperties::ShadowMappingProperties()
{
    ResetToDefaults();
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

void Hachiko::ShadowMappingProperties::Save(YAML::Node& node) const 
{
    node[Uniforms::ShadowMap::LIGHT_BLEEDING_REDUCTION_AMOUNT] = light_bleeding_reduction_amount;
    node[Uniforms::ShadowMap::BIAS] = bias;
    node[Uniforms::ShadowMap::MIN_VARIANCE] = min_variance;
    node[Uniforms::ShadowMap::EXPONENT] = exponent;
    node[Uniforms::Filtering::GAUSSIAN_BLUR_SCALE] = gaussian_filter_blur_amount;
}

void Hachiko::ShadowMappingProperties::Load(const YAML::Node& node) 
{
    // We already set the default values from constructor, no needs to do 
    // unnecessary re-assigning.

    if (node[Uniforms::ShadowMap::LIGHT_BLEEDING_REDUCTION_AMOUNT].IsDefined())
    {
        light_bleeding_reduction_amount = node
            [Uniforms::ShadowMap::LIGHT_BLEEDING_REDUCTION_AMOUNT].as<float>();
    }

    if (node[Uniforms::ShadowMap::BIAS].IsDefined())
    {
        bias = node[Uniforms::ShadowMap::BIAS].as<float>();
    }

    if (node[Uniforms::ShadowMap::MIN_VARIANCE].IsDefined())
    {
        min_variance = node[Uniforms::ShadowMap::MIN_VARIANCE].as<float>();
    }

    if (node[Uniforms::ShadowMap::EXPONENT].IsDefined())
    {
        exponent = node[Uniforms::ShadowMap::EXPONENT].as<float>();
    }

    if (node[Uniforms::Filtering::GAUSSIAN_BLUR_SCALE].IsDefined())
    {
        gaussian_filter_blur_amount = node[Uniforms::Filtering::GAUSSIAN_BLUR_SCALE].as<float>();
    }
}

void Hachiko::ShadowMappingProperties::ResetToDefaults() 
{
    gaussian_filter_blur_amount = ShadowMappingDefaults::GAUSSIAN_FILTER_BLUR_AMOUNT;
    min_variance = ShadowMappingDefaults::MIN_VARIANCE;
    light_bleeding_reduction_amount = ShadowMappingDefaults::LIGHT_BLEEDING_REDUCTION_AMOUNT;
    bias = ShadowMappingDefaults::BIAS;
    exponent = ShadowMappingDefaults::EXPONENT;
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

bool Hachiko::ShadowMappingProperties::DrawEditorContent() 
{
    Widgets::DragFloatConfig config_1;
    config_1.format = "%.9f";
    config_1.speed = 0.0001f;
    config_1.min = 0.0f;

    Widgets::DragFloatConfig config_2 = config_1;
    config_2.max = 1.0f;

    bool changes_made = false;

    if (Widgets::DragFloat("Bias", bias, &config_2))
    {
        changes_made = true;
    }

    if (Widgets::DragFloat("Light Bleeding Reduction", light_bleeding_reduction_amount, &config_2)) 
    {
        changes_made = true;
    }

    if (Widgets::DragFloat("Min Variance", min_variance, &config_2))
    {
        changes_made = true;
    }

    if (Widgets::DragFloat("Exponent", exponent, &config_1))
    {
        changes_made = true;
    }

    if (Widgets::DragFloat("Gaussian Blur Amount", gaussian_filter_blur_amount, &config_1))
    {
        changes_made = true;
    }

    if (ImGui::Button("Reset to Defaults"))
    {
        ResetToDefaults();
        changes_made = true;
    }

    return changes_made;
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
