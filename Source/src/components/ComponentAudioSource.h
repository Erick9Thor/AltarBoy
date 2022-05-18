#pragma once

#include "components/Component.h"
#include "modules/ModuleAudio.h"

namespace Hachiko
{
    class GameObject;
    class ComponentAudioSource : public Component
    {
    public:
        ComponentAudioSource(GameObject* conatiner);
        ~ComponentAudioSource() override;

        void DrawGui() override;

    };
} // namespace Hachiko
