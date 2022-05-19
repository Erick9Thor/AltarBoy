#pragma once

#include "core/preferences/Preferences.h"

namespace Hachiko
{
    class AudioPreferences final : public Preferences
    {
    public:
        AudioPreferences();
        ~AudioPreferences() override = default;
        void LoadConfigurationData(const YAML::Node& node) override;
        void SaveConfigurationData(YAML::Node& node) override;

        [[nodiscard]] float GetSFXVolume() 
        {
            return sfx_volume;
        }
        
        void SetSFXVolume(const float value)
        {
            sfx_volume = value;
        }

        [[nodiscard]] float GetMusicVolume()
        {
            return music_volume;
        }

        void SetMusicVolume(const float value)
        {
            music_volume = value;
        }

    private:
        float sfx_volume = 0.1f;
        float music_volume = 0.3f;
    };
} // namespace Hachiko