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

        void Play(const wchar_t* name_event) const;

        void DrawGui() override;

    private:
        AkTransform source_transform;
        AkGameObjectID GAME_OBJECT_SOURCE;
    };
} // namespace Hachiko
