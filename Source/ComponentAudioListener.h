#pragma once

#include "components/Component.h"
#include "modules/ModuleAudio.h"

namespace Hachiko
{
    class GameObject;
    class ComponentAudioListener : public Component
    {
    public:
        ComponentAudioListener(GameObject* conatiner);
        ~ComponentAudioListener() override;
        
    };
} // namespace Hachiko
