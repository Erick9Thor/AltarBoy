#include "GameObject.h"

#include "Globals.h"
#include "Utils/Logger.h"

#include "Application.h"
#include "Scene.h"
#include "Quadtree.h"
#include "Components/ComponentTransform.h"
#include "Components/ComponentCamera.h"
#include "Components/ComponentMesh.h"
#include "Components/ComponentMaterial.h"

#include <debugdraw.h>


GameObject::GameObject(const char* name)
	: name(name)
{
	AddComponent(new ComponentTransform(this, float3::zero, Quat::identity, float3::one));
}

GameObject::GameObject(GameObject* parent, const float4x4& transform, const char* name)
	: name(name)
{
	SetNewParent(parent);
	AddComponent(new ComponentTransform(this, transform));
}

GameObject::GameObject(GameObject* parent, const char* name, const float3& translation, const Quat& rotation, const float3& scale)
	: name(name)
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
		parent->RemoveChild(this);
	

	if (new_parent)
		new_parent->childs.push_back(this);

	parent = new_parent;
}

void GameObject::AddComponent(Component* component)
{
	switch (component->GetType())
	{
		case (Component::Type::Transform):
		{
			components.push_back(component);
			transform = (ComponentTransform*) component;
			component->SetGameObject(this);
			break;
		}
		case (Component::Type::Camera):
		{
			components.push_back((Component*) component);
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
	case (Component::Type::Transform):
		return transform;
		break;
	case (Component::Type::Camera):
		new_component = new ComponentCamera(this);
		new_component->OnTransformUpdated();
		break;
	case (Component::Type::Mesh):
		new_component = new ComponentMesh(this);
		break;
	case (Component::Type::Material):
		new_component = new ComponentMaterial(this);
		break;
	}
	if (new_component != nullptr)
		components.push_back(new_component);
	else
		LOG("Falied to create component");
	return new_component;
}

void GameObject::RemoveChild(GameObject* game_object)
{
	childs.erase(std::remove(childs.begin(), childs.end(), game_object), childs.end());
}

void GameObject::Destroy()
{
	if (parent)
	{
		parent->RemoveChild(this);
		parent = nullptr;
	}

	if (scene_owner)
		scene_owner->DestroyGameObject(this);

		for (unsigned int i = 0; i < childs.size(); ++i)
		{
			childs[i]->Destroy();
		}
}

void GameObject::Update()
{
		if (transform->HasChanged())
		{
			OnTransformUpdated();
		}

		for (Component* component : components)
		{
			component->Update();
		}

	for (GameObject* child : childs)
	{
		if (child->IsActive())	
			child->Update();
	}
}

void GameObject::DrawAll(ComponentCamera* camera)
{
	// Draw yourself
	Draw(camera);
	// Draw children recursively
	for (GameObject* child : childs)
	{
		child->DrawAll(camera);
	}
}

void GameObject::Draw(ComponentCamera* camera)
{
	// Call draw on all components
	for (Component* component : components)
	{
		component->Draw(camera);
	}
}

void GameObject::OnTransformUpdated()
{
	// Update components
	for (Component* component : components)
		component->OnTransformUpdated();

	// Update children
	for (GameObject* child : childs)
		child->OnTransformUpdated();

	UpdateBoundingBoxes();
}

void GameObject::DebugDrawAll()
{
	// Draw yourself
	DebugDraw();
	// Draw children recursively
	for (GameObject* child : childs)
	{
		child->DebugDrawAll();
	}
}

void GameObject::DebugDraw()
{
	DrawBoundingBox();
	for (Component* component : components)
	{
		component->DebugDraw();
	}
}

void GameObject::DrawBoundingBox()
{
	ddVec3 p[8];
	// This order was pure trial and error, i dont know how to really do it
	// Using center and points does not show the rotation
	static const int order[8] = {0, 1, 5, 4, 2, 3, 7, 6};
	for (int i = 0; i < 8; ++i)
		p[i] = obb.CornerPoint(order[i]);

	dd::box(p, dd::colors::White);
}

void GameObject::UpdateBoundingBoxes()
{
	constexpr float default_bounding_size = 1.0f;
	ComponentMesh* mesh = GetComponent<ComponentMesh>();
	if (mesh != nullptr)
	{
		obb = mesh->GetAABB();
		obb.Transform(transform->GetTransform());
		// Enclose is accumulative, reset the box
		aabb.SetNegativeInfinity();
		aabb.Enclose(obb);
	}
	else
	{
		// If there is no mesh generate a default size
		aabb.SetNegativeInfinity();
		aabb.SetFromCenterAndSize(transform->GetPosition(), float3(default_bounding_size));
		obb = aabb;
	}

	// Without the check main camera crashes bcs there is no quadtree
	if (scene_owner) {
		Quadtree* quadtree = scene_owner->GetQuadtree();
		quadtree->Remove(this);
		quadtree->Insert(this);
	}
}
