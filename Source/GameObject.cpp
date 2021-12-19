#include "GameObject.h"
#include "Globals.h"

#include "ComponentTransform.h"
#include "ComponentCamera.h"

GameObject::GameObject()
{
	AddComponent(new ComponentTransform(this, float3::zero, Quat::identity, float3::one));
}

GameObject::GameObject(GameObject* parent, const float4x4& transform, const char* name) : name(name)
{
	SetNewParent(parent);
	AddComponent(new ComponentTransform(this, transform));
}

GameObject::GameObject(GameObject* parent, const char* name, const float3& translation, const Quat& rotation, const float3& scale) : name(name)
{
	SetNewParent(parent);
	AddComponent(new ComponentTransform(this, translation, rotation, scale));
}

GameObject::~GameObject()
{
	for (unsigned int i = 0; i < childs.size(); i++)
	{
		RELEASE(childs[i]);
	}
	for (unsigned int i = 0; i < components.size(); i++)
	{
		RELEASE(components[i]);
	}
	childs.clear();
}

void GameObject::SetNewParent(GameObject* new_parent)
{
	if (new_parent == parent)
		return;

	if (parent)
		parent->childs.erase(std::remove(childs.begin(), childs.end(), this), childs.end());

	if (new_parent)
		new_parent->childs.push_back(this);

	parent = new_parent;
}

void GameObject::AddComponent(Component* component)
{
	switch (component->GetType()) {
		case(Component::Type::Transform):
		{
			components.push_back(component);
			transform = (ComponentTransform*)component;
			component->SetGameObject(this);
			break;
		}
		case(Component::Type::Camera):
		{
			components.push_back((Component*)component);
			component->SetGameObject(this);
			break;
		}
	}
}

Component* GameObject::CreateComponent(Component::Type type)
{
	Component* new_component = nullptr;
	switch (type)
	{
		case(Component::Type::Transform):
			return transform;
		break;
		case(Component::Type::Camera):
			new_component = new ComponentCamera(this);
			new_component->OnTransformUpdated();
		break;
	}
	if (new_component !=nullptr)
	{
		components.push_back(new_component);
	}
	return new_component;
}

void GameObject::RemoveChild(GameObject* gameObject)
{
	for (vector<GameObject*>::const_iterator it = gameObject->childs.begin(); it != gameObject->childs.end(); ++it)
	{
		if ((*it) == gameObject)
		{
			childs.erase(it);
			break;
		}
	}
}

void GameObject::Destroy()
{
	if (parent)
	{
		parent->RemoveChild(this);
		parent = nullptr;
	}

	if (scene_owner)
		// scene_owner->OnDestroyGameObject(this);

	for (unsigned int i = 0; i < childs.size(); ++i)
	{
		childs[i]->Destroy();
	}
}

void GameObject::Update()
{
	std::vector<Component*>::iterator it;
	for (it = components.begin(); it != components.end(); ++it)
	{
		(*it)->OnUpdate();
		(*it)->OnTransformUpdated();
	}
}