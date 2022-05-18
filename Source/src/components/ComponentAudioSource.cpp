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