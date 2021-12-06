#pragma once

class GameObject;

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

	virtual void OnTransformUpdated() {};

	inline Type GetType() const { return type; };

	GameObject* gameObject = nullptr;

	protected:
		bool active = true;
		Type type = None;
};

