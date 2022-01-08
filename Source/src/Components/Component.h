#pragma once

#include "../Utils/UID.h"
#include "../Importers/JsonFormaterValue.h"

class GameObject;
class ComponentCamera;

class Component
{
public:
	enum Type
	{
		None,
		Transform,
		Mesh,
		Material,
		Camera,
		Unknown
	};

	Component(Type type, GameObject* container)
		: type(type)
		, game_object(container) {}

	virtual ~Component() {};

	virtual void Update() {};
	virtual void Draw(ComponentCamera* camera) {};
	virtual void OnTransformUpdated() {};
	virtual void Save(JsonFormaterValue jComponent) const {};
	virtual void Load(JsonFormaterValue jComponent) {};

	void SetGameObject(GameObject* container)
	{
		game_object = container;
	}

	inline Type GetType() const
	{
		return type;
	};
	const GameObject* GetGameObject() const
	{
		return game_object;
	}
	GameObject* GetGameObject()
	{
		return game_object;
	}

	virtual void DrawGui() {};

protected:
	bool active = true;
	Type type = Type::Unknown;

private:
	UID id = 0;
	GameObject* game_object = nullptr;
};
