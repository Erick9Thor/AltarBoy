#pragma once
#include "MathGeoLib.h"

#include "Utils/UID.h"
#include "Components/Component.h"

#include <vector>
using namespace std;

class ComponentTransform;
class ComponentCamera;
class Program;
class Scene;

class GameObject
{
	friend class Component;

public:
	GameObject(const char* name = "Unnamed");
	GameObject(GameObject* parent, const float4x4& transform, const char* name = "Unnamed", UID uid = GenerateUID());
	GameObject(GameObject* parent, const char* name = "Unnamed", UID uid = GenerateUID(), const float3& translation = float3::zero, const Quat& rotation = Quat::identity, const float3& scale = float3::one);
	virtual ~GameObject();

	void SetNewParent(GameObject* new_parent);
	void AddComponent(Component* component);
	Component* CreateComponent(Component::Type type);
	void RemoveChild(GameObject* gameObject);

	void Destroy();
	void Update();
	void DrawAll(ComponentCamera* camera, Program* program);
	void Draw(ComponentCamera* camera, Program* program);

	inline bool IsActive() { return active; }

	void OnTransformUpdated();

	void DebugDrawAll();
	void DebugDraw();
	void DrawBoundingBox();
	void UpdateBoundingBoxes();

	UID getUID() { return uid; }
	void Save(JsonFormaterValue j_gameObject) const;
	void Load(JsonFormaterValue j_gameObject);

	const OBB& GetOBB() const { return obb; }
	const AABB& GetAABB() { return aabb; }
	const std::vector<Component*>& GetComponents() const { return components; }

	template<typename RetComponent>
	RetComponent* GetComponent()
	{
		Component::Type type = RetComponent::GetType();
		for (unsigned int i = 0; i < components.size(); i++)
		{
			if (components[i]->GetType() == type)
				return ((RetComponent*) (components[i]));
		}
		return nullptr;
	}

	std::string name;
	Scene* scene_owner = nullptr;
	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;

	bool hierarchy_open = false;
	bool active = true;

private:
	std::vector<Component*> components;
	ComponentTransform* transform = nullptr;

	AABB aabb;
	OBB obb;

	UID uid = 0;
};