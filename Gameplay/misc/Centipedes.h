#pragma once

#include <scripting/Script.h>

namespace Hachiko
{
	namespace Scripting
	{
		class AudioManager;

		class Centipedes : public Script
		{
			SERIALIZATION_METHODS(false)

			enum class CentipedeState {
				IDLE,
				RUN,
				DIE
			};

		public:
			Centipedes(GameObject* game_object);
			~Centipedes() override = default;

			void OnAwake() override;
			void OnUpdate() override;

			CentipedeState GetState() {
				return _state;
			}

		private:

			void UpdateCentipedeState();
			void SteppedOn();

			CentipedeState _state = CentipedeState::IDLE;
			CentipedeState _previous_state = CentipedeState::IDLE;

			float player_range = 10.0f;
			float stepped_on_range = 1.0f;
			float rotation_smoothness = 0.2f;
			float time_to_respawn = 10.0f;

			float centipide_aggro = 40.0f;

			float respawn_timer = 0.0f;

			GameObject* _player = nullptr;

			ComponentTransform* player_transform = nullptr;
			ComponentAgent* agent = nullptr;
			ComponentAnimation* animation = nullptr;
		};
	}
}


