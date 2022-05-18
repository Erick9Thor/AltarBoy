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
        
        void OnTransformUpdated() override;

        void DrawGui() override;  

    private:
        AkTransform listener_transform;
        AkGameObjectID MY_DEFAULT_LISTENER = 0;

    };
} // namespace Hachiko
