#include "core/hepch.h"

#include "ComponentAnimation.h"
#include "ComponentTransform.h"

#include "resources/ResourceAnimation.h"
#include "animation/AnimationController.h"
#include "importers/AnimationImporter.h"

Hachiko::ComponentAnimation::ComponentAnimation(GameObject* container) : Component(Type::ANIMATION, container)
{
    controller = new AnimationController();
}

Hachiko::ComponentAnimation::~ComponentAnimation()
{
    delete controller;

    for (unsigned i = 0; i < animations.size(); ++i)
    {
        delete animations[i];
    }
    animations.clear();
}

void Hachiko::ComponentAnimation::Start()
{
    if (!animations.empty())
    {
        controller->Play(animations[0]->GetID(), true, 0);
    }
}

void Hachiko::ComponentAnimation::Stop()
{
    controller->Stop();
}

void Hachiko::ComponentAnimation::Update()
{
    controller->Update(EngineTimer::delta_time); // TODO: change for GameTimer::delta_time

    if (game_object != nullptr)
    {
        UpdatedGameObject(game_object);
    }
}

void Hachiko::ComponentAnimation::UpdatedGameObject(GameObject* go)
{
    float3 position;
    Quat rotation;

    if (controller->GetTransform(controller->current, go->name.c_str(), position, rotation))
    {
        go->GetTransform()->SetLocalPosition(position);
        go->GetTransform()->SetLocalRotation(rotation);
    }

    for (GameObject* child : go->children)
    {
        UpdatedGameObject(child);
    }
}

void Hachiko::ComponentAnimation::DrawGui()
{
    ImGui::PushID(this);
    if (ImGuiUtils::CollapsingHeader(game_object, this, "Animation"))
    {
        if (ImGui::Checkbox("Active Debug", &active))
        {
            /* TODO */
        }
    }
    ImGui::PopID();
}

void Hachiko::ComponentAnimation::Save(YAML::Node& node) const {}

void Hachiko::ComponentAnimation::Load(const YAML::Node& node) {}
