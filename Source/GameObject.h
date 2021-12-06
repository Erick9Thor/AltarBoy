#pragma once
#include "Math.h"

#include "Component.h"
#include <vector>;
using namespace std;

class ComponentTransform;

class GameObject
{
	public:
		GameObject();
		GameObject(GameObject* parent, const float4x4& transform, const char* name);
		~GameObject();

		void AddComponent(Component* component);
		Component* CreateComponent(Component::Type type);

		template<typename RetComponent>
		RetComponent* GetComponent()
		{
			
			// TODO
			return nullptr;
		}

		GameObject* parent = nullptr;
		std::vector<GameObject*> childs;
	private:
		std::vector<Component*> components;
		ComponentTransform* transform = nullptr;
		std::string	name;
};

