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

}

/**     GUI     **/

void Hachiko::ComponentAudioSource::DrawGui()
{
    ImGui::PushID(this);


    if (ImGuiUtils::CollapsingHeader(game_object, this, "Audio Source"))
    {
        ImGui::Text("Holi");
    }
    ImGui::PopID();
}