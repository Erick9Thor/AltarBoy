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

        void OnTransformUpdated() override;

        void DrawGui() override;

    private:
        AkTransform source_transform;
        AkGameObjectID GAME_OBJECT_ID_ = 1000;
    };
} // namespace Hachiko
