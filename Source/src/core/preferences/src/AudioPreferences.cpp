#include "core/hepch.h"
#include "AudioPreferences.h"

using namespace Hachiko;

AudioPreferences::AudioPreferences() : Preferences(Type::GLOBALS)
{
    group_name = AUDIO_NODE;
}

void AudioPreferences::LoadConfigurationData(const YAML::Node& node)
{
    for (auto it = node.begin(); it != node.end(); ++it)
    {
        if (it->first.as<std::string>()._Equal(SFX_VOLUME))
        {
            sfx_volume = it->second.as<float>();
            continue;
        }

        if (it->first.as<std::string>()._Equal(MUSIC_VOLUME))
        {
            music_volume = it->second.as<float>();
        }
    }
}

void AudioPreferences::SaveConfigurationData(YAML::Node& node)
{
    node[group_name][SFX_VOLUME] = sfx_volume;
    node[group_name][MUSIC_VOLUME] = music_volume;
}