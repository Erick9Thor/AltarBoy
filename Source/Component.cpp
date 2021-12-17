#include "Component.h"

Component::Component(Type type, GameObject* container): type(type), game_object(container)
{
}

Component::~Component()
{
}

const GameObject* Component::GetGameObject() const
{
	return game_object;
}

GameObject* Component::GetGameObject()
{
	return game_object;
}