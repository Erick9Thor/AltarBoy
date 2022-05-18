#include "core/hepch.h"
#include "ComponentAudioSource.h"
#include "components/ComponentTransform.h"

/**     CONSTRUCTORS    **/

Hachiko::ComponentAudioSource::ComponentAudioSource(GameObject* container)
    : Component(Type::AUDIO_SOURCE, container)
{

}

Hachiko::ComponentAudioSource::~ComponentAudioSource()
{

}