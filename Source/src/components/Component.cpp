#include "core/hepch.h"

bool Hachiko::Component::CanBeRemoved() const
{
    return !HasDependentComponents(game_object);
}

bool Hachiko::Component::HasDependentComponents(GameObject* game_object) const
{
    return false;
}
