#pragma once

#include "components/Component.h"

class AkTransform;

namespace Hachiko
{
    class GameObject;
    
    class ComponentAudioSource : public Component
    {
    public:
        ComponentAudioSource(GameObject* conatiner);
        ~ComponentAudioSource() override;

        void OnTransformUpdated() override;

        HACHIKO_API void PostEvent(const wchar_t* name_event) const;

        void DrawGui() override;

    private:
        AkTransform* source_transform;
        uint64_t source_id; // This is AkGameObjectID, actually same with uint64_t
    };
} // namespace Hachiko
