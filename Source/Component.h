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
			Unknown
		};

		Component(Type type, GameObject* container);
		virtual ~Component();

		inline Type GetType() const { return type; };

		void SetGameObject(GameObject* container) { game_object = container; }
		const GameObject* GetGameObject() const;
		GameObject* GetGameObject();

		virtual void OnTransformUpdated() {};
		virtual void OnUpdate() {};

	protected:
		GameObject* game_object = nullptr;
		bool active = true;
		Type type = None;
};

