#pragma once

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

	Component(Type type, GameObject* container) : type(type) , game_object(container) {}
	virtual ~Component() {};

	inline Type GetType() const { return type; };

	virtual void Draw(ComponentCamera* camera) {};
	virtual void DebugDraw() {};	

	void SetGameObject(GameObject* container) { game_object = container; }

	const GameObject* GetGameObject() const { return game_object; }
	GameObject* GetGameObject() { return game_object; }

	virtual void Update() {};
	virtual void OnTransformUpdated() {};

	virtual void DrawGui() {};

protected:
	GameObject* game_object = nullptr;
	bool active = true;
	Type type = Type::Unknown;
};
