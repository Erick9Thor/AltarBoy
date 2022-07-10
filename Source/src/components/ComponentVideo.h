#pragma once

#include "Component.h"

namespace Hachiko
{
    class ComponentVideo : public Component
    {
    public:
        ComponentVideo(GameObject* container);
        ~ComponentVideo() override = default;

        void DrawGui() override;

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;

        HACHIKO_API void Play();
        HACHIKO_API void Pause();
        HACHIKO_API void Stop();

    private:
    };
} // namespace Hachiko