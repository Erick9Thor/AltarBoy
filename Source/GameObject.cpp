#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
	for (unsigned int i = 0; i < childs.size(); i++)
	{
		delete childs[i];
		childs[i] = nullptr;
	}
	childs.clear();
}

void GameObject::CreateComponent(Component* component)
{
	switch (component->GetType()) {
		case(Component::Type::Transform):
		{
			components.push_back(component);
			transform = (C_Transform*)component;
			component->gameObject = this;
			break;
		}
		case(Component::Type::Camera):
		{
			components.push_back((Component*)component);
			component->gameObject = this;
			break;
		}
	}
}
