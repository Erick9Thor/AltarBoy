#include "GameObject.h"
#include "Globals.h"

#include "ComponentTransform.h"
#include "ComponentCamera.h"

GameObject::GameObject()
{
	AddComponent(new ComponentTransform(this, float4x4::identity));
}

GameObject::GameObject(GameObject* parent, const char* name, const float3& translation, const Quat& rotation, const float3& scale) : name(name)
{
	this->parent = parent;
	if (parent)
		parent->childs.push_back(this);

	AddComponent(new ComponentTransform(this, translation, rotation, scale));
}

GameObject::GameObject(GameObject* parent, const float4x4& transform, const char* name) : name(name)
{
	this->parent = parent;
	if (parent) {
		parent->childs.push_back(parent);
	}
	AddComponent(new ComponentTransform(this, transform));
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

void GameObject::AddComponent(Component* component)
{
	switch (component->GetType()) {
		case(Component::Type::Transform):
		{
			components.push_back(component);
			transform = (ComponentTransform*)component;
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

Component* GameObject::CreateComponent(Component::Type type)
{
	Component* new_component = nullptr;
	switch (type)
	{
		case(Component::Type::Transform):
		{
			return transform;
		}
		case(Component::Type::Camera):
		{
			new_component = new ComponentCamera(this);
			new_component->OnTransformUpdated();
			break;
		}
	}
	if (new_component)
	{
		components.push_back(new_component);
	}
	return new_component;
}

void GameObject::Update()
{
	if (transform)
	{
		OnTransformUpdated();
	}

	std::vector<Component*>::iterator it;
	for (it = components.begin(); it != components.end(); ++it)
	{
		(*it)->Update();
	}
}

void GameObject::OnTransformUpdated() {
	UpdateAABB();
}

const AABB& GameObject::GetAABB() const
{
	return aabb;
}

const OBB& GameObject::GetOBB() const
{
	return obb;
}

void GameObject::UpdateAABB()
{
	//TODO: Find the way to convert model aabb
}