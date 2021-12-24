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
		GameObject(GameObject* parent, const float4x4& transform, const char* name = "Unnamed");
		GameObject(GameObject* parent, const char* name = "Unnamed", const float3& translation = float3::zero, const Quat& rotation = Quat::identity, const float3& scale = float3::one);
		virtual ~GameObject();

		void SetNewParent(GameObject* new_parent);

		void AddComponent(Component* component);
		Component* CreateComponent(Component::Type type);
		void RemoveChild(GameObject* gameObject);

		void Destroy();

		void Update();

		void DrawAll();
		void Draw();

		void OnTransformUpdated();
		
		inline bool IsEnabled() { return enabled; }

		const std::vector<Component*>& GetComponents() const { return components; }

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
		
		std::string	name;
		bool hierarchy_open = false;

	private:
		bool enabled = true;
		std::vector<Component*> components;
		ComponentTransform* transform = nullptr;

		AABB aabb;
		OBB	obb;
};