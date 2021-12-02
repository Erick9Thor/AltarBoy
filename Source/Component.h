#pragma once

#include "GameObject.h";

class Component
{
	public:
		enum Type
		{
			None,
			Transform,
			Mesh,
			Texture,
			Camera,
		};

	Component(Type type, GameObject*);
	virtual ~Component();

	GameObject* gameObject = nullptr;

	protected:
		bool active = true;
		Type type = None;
};

