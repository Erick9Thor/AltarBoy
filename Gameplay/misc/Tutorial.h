#pragma once

#include <scripting/Script.h>
#include "Gameplay.h"

namespace Hachiko
{
	class GameObject;
	namespace Scripting
	{
		class TimeManager;
		class PlayerController;
		class EnemyController;
		class LevelManager;

		class Tutorial : public Script
		{
			SERIALIZATION_METHODS(false)

		public:
			Tutorial(GameObject* game_object);
			~Tutorial() override = default;

			void OnAwake() override;
			void OnStart() override;
			void OnUpdate() override;

		private:
			SERIALIZE_FIELD(GameObject*, _first_enemy);

			TimeManager* _time_manager;
			GameObject* _player = nullptr;
			GameObject* _player_tutorial = nullptr;
			GameObject* _parasite_tutorial = nullptr;
			PlayerController* _player_controller;
			EnemyController* _enemy_controller;
			LevelManager* _level_manager;

			bool _tutorial_showed = false;
		};
	} // namespace Scripting
} // namespace Hachiko
