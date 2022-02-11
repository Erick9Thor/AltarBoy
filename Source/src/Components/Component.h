#pragma once

#include "Utils/UUID.h"
#include "Utils/JsonFormaterValue.h"

class GameObject;
class ComponentCamera;
class Program;

class Component
{
public:
	enum Type
	{
		NONE,
		TRANSFORM,
		MESH,
		MATERIAL,
		CAMERA,
		DIRLIGHT,
		POINTLIGHT,
		SPOTLIGHT,
		UNKNOWN
	};

	Component(Type type, GameObject* container)
		: type(type)
		, game_object(container) {}

	virtual ~Component() {};

	virtual void Update() {};
	virtual void OnTransformUpdated() {};

	inline Type GetType() const { return type; };
    Hachiko::UID GetID() const { return uid; }

	const GameObject* GetGameObject() const { return game_object; }
	GameObject* GetGameObject() { return game_object; }

	void SetGameObject(GameObject* container) { game_object = container; }

	virtual void DrawGui() {};

	virtual void Draw(ComponentCamera* camera, Program* program) {};
	virtual void DebugDraw() {};

	virtual void Save(JsonFormaterValue j_component) const {}; 
	virtual void Load(JsonFormaterValue j_component) {};

protected:
	GameObject* game_object = nullptr;
	bool active = true;
	Type type = Type::UNKNOWN;

private:
    Hachiko::UID uid = 0; 
};
