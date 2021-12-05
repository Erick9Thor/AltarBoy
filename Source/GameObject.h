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
		~GameObject();

		void CreateComponent(Component* component);

		GameObject* parent = nullptr;
		std::vector<GameObject*> childs;
	private:
		std::vector<Component*> components;
		ComponentTransform* transform = nullptr;
};

