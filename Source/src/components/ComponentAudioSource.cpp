#include "core/hepch.h"
#include "ComponentAudioSource.h"
#include "components/ComponentTransform.h"

/**     CONSTRUCTORS    **/

Hachiko::ComponentAudioSource::ComponentAudioSource(GameObject* container)
    : Component(Type::AUDIO_SOURCE, container)
{
    GAME_OBJECT_SOURCE = GetID();
    AK::SoundEngine::RegisterGameObj(GAME_OBJECT_SOURCE, game_object->GetName().c_str());
    OnTransformUpdated();
    Play(L"Play_BackgroundMusic"); // Unncoment this to test, should be erased at the final version
}

Hachiko::ComponentAudioSource::~ComponentAudioSource()
{
    AK::SoundEngine::StopAll(GAME_OBJECT_SOURCE);
    AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_SOURCE);
}

void Hachiko::ComponentAudioSource::OnTransformUpdated()
{
    ComponentTransform* transform = game_object->GetTransform();

    const float3& pos = transform->GetGlobalPosition();
    const float3& front = transform->GetFront();
    const float3& up = transform->GetUp();

    source_transform.Set(pos.x, pos.y, pos.z,
                         front.x, front.y, front.z,
                         up.x, up.y, up.z
    );

    AK::SoundEngine::SetPosition(GAME_OBJECT_SOURCE, source_transform);

}

void Hachiko::ComponentAudioSource::PostEvent(const wchar_t* name_event) const
{
    AK::SoundEngine::PostEvent(name_event, GAME_OBJECT_SOURCE);
}

/**     GUI     **/

void Hachiko::ComponentAudioSource::DrawGui()
{
    ImGui::PushID(this);

    if (ImGuiUtils::CollapsingHeader(game_object, this, "Audio Source"))
    {
        ImGui::Text("Now you can call events to be played ^-^");
    }
    ImGui::PopID();
}