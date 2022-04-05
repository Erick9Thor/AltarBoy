#include "core/hepch.h"
#include "core/GameObject.h"

bool Hachiko::Component::CanBeRemoved() const
{
    return !HasDependentComponents(game_object);
}

bool Hachiko::Component::HasDependentComponents(GameObject* game_object) const
{
    return false;
}
