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
			Material,
			Camera,
			Unknown
		};

		Component(Type type, GameObject* container) : type(type), game_object(container) {}
		virtual ~Component() {};

		virtual void Draw() {};

		inline Type GetType() const { return type; };

		void SetGameObject(GameObject* container) { game_object = container; }
		const GameObject* GetGameObject() const { return game_object; }
		GameObject* GetGameObject() { return game_object; }

		virtual void Update() {};

		virtual void OnTransformUpdated() {};

		virtual void DrawGui() {};

	protected:
		GameObject* game_object = nullptr;
		bool active = true;
		Type type = None;
};

