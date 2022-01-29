#include "GameObject.h"

#include "Globals.h"
#include "Utils/Logger.h"

#include "Application.h"
#include "Scene.h"
#include "Quadtree.h"
#include "Program.h"
#include "Components/ComponentTransform.h"
#include "Components/ComponentCamera.h"
#include "Components/ComponentMesh.h"
#include "Components/ComponentMaterial.h"
#include "Components/ComponentDirLight.h"
#include "Components/ComponentPointLight.h"
#include "Components/ComponentSpotLight.h"

#include <debugdraw.h>


GameObject::GameObject(const char* name)
	: name(name)
{
	AddComponent(new ComponentTransform(this, float3::zero, Quat::identity, float3::one));
}

GameObject::GameObject(GameObject* parent, const float4x4& transform, const char* name, UID uid)
	: name(name)
	, uid(uid)
{
	SetNewParent(parent);
	AddComponent(new ComponentTransform(this, transform));
}

GameObject::GameObject(GameObject* parent, const char* name, UID uid, const float3& translation, const Quat& rotation, const float3& scale)
	: name(name)
	, uid(uid)
{
	SetNewParent(parent);
	AddComponent(new ComponentTransform(this, translation, rotation, scale));
}

GameObject::~GameObject()
{
	if (parent)
		parent->RemoveChild(this);

	if (scene_owner)
		scene_owner->DestroyGameObject(this);
	
	for (GameObject* child : childs)
	{
		child->SetNewParent(nullptr);
		RELEASE(child);
	}

	for (Component* component : components)
	{
		RELEASE(component);
	}
}

void GameObject::RemoveChild(GameObject* game_object)
{
	childs.erase(std::remove(childs.begin(), childs.end(), game_object), childs.end());
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
		case (Component::Type::TRANSFORM):
		{
			components.push_back(component);
			transform = (ComponentTransform*) component;
			component->SetGameObject(this);
			break;
		}
		case (Component::Type::CAMERA):
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
	case (Component::Type::TRANSFORM):
		return transform;
		break;
	case (Component::Type::CAMERA):
		new_component = new ComponentCamera(this);
		break;
	case (Component::Type::MESH):
		new_component = new ComponentMesh(this);
		break;
	case (Component::Type::MATERIAL):
		new_component = new ComponentMaterial(this);
		break;
	case (Component::Type::DIRLIGHT):
		new_component = new ComponentDirLight(this);
		break;
	case (Component::Type::POINTLIGHT):
		new_component = new ComponentPointLight(this);
		break;
	case (Component::Type::SPOTLIGHT):
		new_component = new ComponentSpotLight(this);
		break;
	}

	if (new_component != nullptr)
		components.push_back(new_component);
	else
		LOG("Falied to create component");
	return new_component;
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

void GameObject::DrawAll(ComponentCamera* camera, Program* program)
{
	// Draw yourself
	Draw(camera, program);
	// Draw children recursively
	for (GameObject* child : childs)
	{
		child->DrawAll(camera, program);
	}
}

void GameObject::Draw(ComponentCamera* camera, Program* program)
{
	// Call draw on all components
	for (Component* component : components)
	{
		component->Draw(camera, program);
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

void GameObject::RemoveComponent(Component* component)
{
	//TODO: Should I delete the component?
	components.erase(std::remove(components.begin(), components.end(), component));
}

void GameObject::Save(JsonFormaterValue j_gameObject) const
{
	j_gameObject["Uid"] = uid;
	j_gameObject["GOName"] = name.c_str();
	j_gameObject["Active"] = active;
	j_gameObject["ParentId"] = parent != nullptr ? parent->uid : 0;

	JsonFormaterValue j_components = j_gameObject["Components"];
	for (unsigned i = 0; i < components.size(); ++i)
	{
		JsonFormaterValue j_component = j_components[i];
		Component* component = components[i];

		j_component["ComponentID"] = component->GetID();
		j_component["ComponentType"] = component->GetType();
		component->Save(j_component);
	}

	JsonFormaterValue j_children = j_gameObject["GOChildrens"];
	for (unsigned i = 0; i < childs.size(); ++i)
	{
		JsonFormaterValue j_child = j_children[i];
		GameObject* child = childs[i];
		child->Save(j_child);
	}
}

void GameObject::Load(JsonFormaterValue j_gameObject)
{
	JsonFormaterValue j_components = j_gameObject["Components"];
	for (unsigned i = 0; i < j_components.Size(); ++i)
	{
		JsonFormaterValue j_component = j_components[i];

		UID c_uid = j_component["ComponentID"];
		int enum_type = j_component["ComponentType"];
		bool active = j_component["Active"];

		Component::Type type = Component::Type(enum_type);

		Component* component = CreateComponent(type);

		component->Load(j_component);
	}

	JsonFormaterValue j_childrens = j_gameObject["GOChildrens"];
	for (unsigned i = 0; i < j_childrens.Size(); ++i)
	{
		JsonFormaterValue j_child = j_childrens[i];

		std::string child_name = j_child["GOName"];
		GameObject* child = new GameObject(this, child_name.c_str(), j_child["Uid"]);
		child->scene_owner = scene_owner;
		child->Load(j_child);

	}
}


