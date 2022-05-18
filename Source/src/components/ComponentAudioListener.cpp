#include "core/hepch.h"
#include "ComponentAudioListener.h"
#include "components/ComponentTransform.h"

/**     CONSTRUCTORS    **/

Hachiko::ComponentAudioListener::ComponentAudioListener(GameObject* container) 
    : Component(Type::AUDIO_LISTENER, container)
{
    AK::SoundEngine::RegisterGameObj(MY_DEFAULT_LISTENER, "My Default Listener");
    AK::SoundEngine::SetDefaultListeners(&MY_DEFAULT_LISTENER, 1);
    OnTransformUpdated();
}

Hachiko::ComponentAudioListener::~ComponentAudioListener()
{

}


void Hachiko::ComponentAudioListener::OnTransformUpdated()
{
    ComponentTransform* transform = game_object->GetTransform();

    const float3& pos = transform->GetGlobalPosition();
    const float3& front = transform->GetFront();
    const float3& up = transform->GetUp();

    listener_transform.Set(pos.x, pos.y, pos.z, 
                           front.x, front.y, front.z, 
                           up.x, up.y, up.z
    );

}

/**     GUI     **/

void Hachiko::ComponentAudioListener::DrawGui() 
{
    ImGui::PushID(this);


    if (ImGuiUtils::CollapsingHeader(game_object, this, "Audio Listener"))
    {
        ImGui::Text("Holi");
    }
    ImGui::PopID();
}
