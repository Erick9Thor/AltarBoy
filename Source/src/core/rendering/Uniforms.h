#pragma once

namespace Hachiko::Uniforms
{

    namespace ShadowMap
    {
        constexpr const char* BIAS = "shadow_bias";
        constexpr const char* EXPONENT = "exponent";
        constexpr const char* MIN_VARIANCE = "min_variance";
        constexpr const char* LIGHT_BLEEDING_REDUCTION_AMOUNT = "light_bleeding_reduction_amount";
        constexpr const char* LIGHT_PROJECTION = "light_projection";
        constexpr const char* LIGHT_VIEW = "light_view";

    } // namespace ShadowMap

    namespace Filtering
    {
        constexpr const char* GAUSSIAN_BLUR_SCALE = "blur_scale";
    } // namespace Filtering

} // namespace Hachiko::Uniforms
