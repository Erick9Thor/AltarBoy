#pragma once
#include "Utils/Math.h"
#include "Components/Component.h"

#include <vector>
using namespace std;

class ComponentTransform;
class ComponentCamera;
class Scene;

class GameObject
{
	friend class Component;

public:
	GameObject(const char* name = "Unnamed");
	GameObject(GameObject* parent, const float4x4& transform, const char* name = "Unnamed");
	GameObject(GameObject* parent, const char* name = "Unnamed", const float3& translation = float3::zero, const Quat& rotation = Quat::identity, const float3& scale = float3::one);
	virtual ~GameObject();
	inline bool IsEnabled()
	{
		return enabled;
	}

	void SetNewParent(GameObject* new_parent);
	void AddComponent(Component* component);
	Component* CreateComponent(Component::Type type);
	void RemoveChild(GameObject* gameObject);

	void Destroy();
	void Update();
	void DrawAll(ComponentCamera* camera);
	void Draw(ComponentCamera* camera);

	void OnTransformUpdated();

	// BOUNDING BOX
	// TODO: Define draw flags
	void DebugDrawAll();
	void DebugDraw();
	void DrawBoundingBox();
	void UpdateBoundingBoxes();

	const OBB& GetOBB() const
	{
		return obb;
	};

	const AABB& GetAABB()
	{
		return aabb;
	}

	const std::vector<Component*>& GetComponents() const
	{
		return components;
	}

	template<typename RetComponent>
	const RetComponent* GetComponent() const
	{
		Component::Type type = RetComponent::GetType();
		for (unsigned int i = 0; i < components.size(); i++)
		{
			if (components[i]->GetType() == type)
			{
				return ((RetComponent*) (components[i]));
			}
		}
		return nullptr;
	}

	template<typename RetComponent>
	RetComponent* GetComponent()
	{
		Component::Type type = RetComponent::GetType();
		for (unsigned int i = 0; i < components.size(); i++)
		{
			if (components[i]->GetType() == type)
			{
				return ((RetComponent*) (components[i]));
			}
		}
		return nullptr;
	}

	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;

	Scene* scene_owner = nullptr;

	std::string name;
	bool hierarchy_open = false;

private:
	bool enabled = true;
	std::vector<Component*> components;
	ComponentTransform* transform = nullptr;

	AABB aabb;
	OBB obb;
};