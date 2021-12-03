#include "Component.h"

Component::Component(Type type, GameObject*): type(type), gameObject(gameObject)
{
}

Component::~Component()
{
}
