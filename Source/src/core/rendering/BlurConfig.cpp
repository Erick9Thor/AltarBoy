#include "core/hepch.h"
#include "BlurConfig.h"

void Hachiko::BlurConfigToEditorUI(BlurConfig* blur_config)
{
    if (blur_config == nullptr)
    {
        return;
    }

    static Widgets::DragFloatConfig config_1;
    config_1.format = "%.9f";
    config_1.speed = 0.01f;
    config_1.min = 0.0f;
    config_1.max = FLT_MAX;

    DragFloat("Gaussian blur intensity", blur_config->intensity, &config_1);
    DragFloat("Gaussian blur sigma", blur_config->sigma, &config_1);

    int current_index = BlurPixelSize::ToIndex(blur_config->size);
    if (Widgets::Combo("Gaussian blur pixel size", &current_index, BlurPixelSize::blur_pixel_sizes_strings, BlurPixelSize::number_of_blur_pixel_sizes))
    {
        blur_config->size = BlurPixelSize::FromIndex(current_index);
    }
}