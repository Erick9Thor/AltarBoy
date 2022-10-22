#pragma once

#include "core/rendering/BloomManager.h"

namespace Hachiko
{
    struct BlurConfig
    {
        float intensity = 0.85f;
        float sigma = 1.45f;
        BlurPixelSize::Type size = BlurPixelSize::Type::Gaussian5x5;
    };

    void BlurConfigToEditorUI(BlurConfig* blur_config);
}