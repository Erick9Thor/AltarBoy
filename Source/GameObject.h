#pragma once

#include "Math.h"

#include <vector>;
using namespace std;

class GameObject
{
	public:
		GameObject();
		~GameObject();

		GameObject* parent = nullptr;
		std::vector<GameObject*>	childs;
};

