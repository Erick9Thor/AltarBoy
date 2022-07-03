#pragma once

#include <scripting/Script.h>

namespace Hachiko
{
	class GameObject;
	class ComponentTransform;
	namespace Scripting
	{
		class LaserController : public Script
		{
			SERIALIZATION_METHODS(false)

				enum State
			{
				ACTIVE = 0,
				ACTIVATING = 1,
				INACTIVE = 2
			};

		// Methods
		public:
			LaserController(GameObject* game_object);
			~LaserController() override = default;

			void OnAwake() override;
			void OnUpdate() override;

		private:
			void ChangeState(State new_state);
			void AdjustLength();
			void CheckPlayerCollision();

		// Variables
		public:
			SERIALIZE_FIELD(GameObject*, _laser);
			SERIALIZE_FIELD(float, _max_length);
			SERIALIZE_FIELD(float, _max_scale);
			SERIALIZE_FIELD(float, _activation_time);
			SERIALIZE_FIELD(float, _damage);

		private:
			State _state = State::ACTIVE;
			float _elapse_time = 0.0f;
			float _length = 0.0f;
			float _scale = 0.2f;

			GameObject* _terrain;
			GameObject* _player;
		};
	}
	
}



