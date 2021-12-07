#pragma once
#include "Math.h"

#include "Component.h"

#include <vector>
using namespace std;

class ComponentTransform;
class Scene;

class GameObject
{
	friend class Component;

	public:
		GameObject();
		GameObject(GameObject* parent, const char* name = "Unnamed", const float3& translation = float3::zero, const Quat& rotation = Quat::identity, const float3& scale = float3::one);
		GameObject(GameObject* parent, const float4x4& transform, const char* name = "Unnamed");
		virtual ~GameObject();

		void SetNewParent(GameObject* new_parent);

		void AddComponent(Component* component);
		Component* CreateComponent(Component::Type type);

		void Update();

		void OnTransformUpdated();
		const AABB& GetAABB() const;
		const OBB& GetOBB() const;
		void UpdateAABB();

		inline ComponentTransform* GetTransform() { return transform; }

		template<typename RetComponent>
		const RetComponent* GetComponent() const
		{
			Component::Type type = RetComponent::GetType();
			for (unsigned int i = 0; i < components.size(); i++)
			{
				if (components[i]->GetType() == type)
				{
					return ((RetComponent*)(components[i]));
				}
			}
			return nullptr;
		}

		template<typename RetComponent>
		RetComponent* GetComponent()
		{
			Component::Type type = RetComponent::GetType();
			if (type == Component::Type::Transform)
				return (RetComponent*)transform;
			for (unsigned int i = 0; i < components.size(); i++)
			{
				if (components[i]->GetType() == type)
				{
					return ((RetComponent*)(components[i]));
				}
			}
			return nullptr;
		}

		GameObject* parent = nullptr;
		std::vector<GameObject*> childs;

		Scene* scene_owner = nullptr;
	private:
		std::vector<Component*> components;
		ComponentTransform* transform = nullptr;
		std::string	name;

		AABB aabb;
		OBB	obb;
};