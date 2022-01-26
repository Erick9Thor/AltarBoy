#pragma once

#include "../Utils/UID.h"
#include "../Utils/JsonFormaterValue.h"
#include "../UI/ImGuiUtils.h"

class GameObject;
class ComponentCamera;
class Program;

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
		DirLight,
		PointLight,
		SpotLight,
		Unknown
	};

	Component(Type type, GameObject* container)
		: type(type)
		, game_object(container) {}

	virtual ~Component() {};

	inline Type GetType() const { return type; };

	virtual void Draw(ComponentCamera* camera, Program* program) {};
	virtual void DebugDraw() {};

	void SetGameObject(GameObject* container) { game_object = container; }

	const GameObject* GetGameObject() const { return game_object; }
	GameObject* GetGameObject() { return game_object; }

	virtual void Update() {};
	virtual void OnTransformUpdated() {};

	virtual void DrawGui() {};

	virtual void Save(JsonFormaterValue j_component) const {}; 
	virtual void Load(JsonFormaterValue j_component) {};

	UID GetID() const { return uid; }

protected:
	GameObject* game_object = nullptr;
	bool active = true;
	Type type = Type::Unknown;

private:
	UID uid = 0; 
};
