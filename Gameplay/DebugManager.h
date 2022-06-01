#pragma once
#include <scripting/Script.h>
#include "Stats.h"
#include "PlayerState.h"

namespace Hachiko
{
	class ComponentButton;
	class GameObject;

	namespace Scripting
	{
		class PlayerController;
		class DebugManager : public Script
		{
			SERIALIZATION_METHODS(false)

		public:
			DebugManager(GameObject* game_object);
			~DebugManager() override = default;

			void OnAwake() override;
			void OnStart() override;
			void OnUpdate() override;

		private:
			SERIALIZE_FIELD(GameObject*, _player);
			SERIALIZE_FIELD(ComponentButton*, _button_back);
			SERIALIZE_FIELD(ComponentButton*, _teleport_next_pos);
			SERIALIZE_FIELD(ComponentButton*, _teleport_prev_pos);
			SERIALIZE_FIELD(ComponentButton*, _teleport_add_pos);
			SERIALIZE_FIELD(ComponentButton*, _add_health);
			SERIALIZE_FIELD(ComponentButton*, _remove_health);
			SERIALIZE_FIELD(ComponentButton*, _increase_max_hp);
			SERIALIZE_FIELD(ComponentButton*, _decrease_max_hp);
			SERIALIZE_FIELD(ComponentButton*, _increase_move_speed);
			SERIALIZE_FIELD(ComponentButton*, _decrease_move_speed);
			SERIALIZE_FIELD(ComponentButton*, _increase_attack_cd);
			SERIALIZE_FIELD(ComponentButton*, _decrease_attack_cd);
			SERIALIZE_FIELD(ComponentButton*, _increase_attack_power);
			SERIALIZE_FIELD(ComponentButton*, _decrease_attack_power);
			SERIALIZE_FIELD(ComponentButton*, _god_mode);
			SERIALIZE_FIELD(ComponentButton*, _flying_mode);
			SERIALIZE_FIELD(ComponentButton*, _spawn_enemy);
			SERIALIZE_FIELD(ComponentButton*, _unlock_skills);
			SERIALIZE_FIELD(ComponentButton*, _toggle_performance_output);
			SERIALIZE_FIELD(ComponentButton*, _toggle_wireframe);
			SERIALIZE_FIELD(ComponentButton*, _toggle_show_colliders);
			SERIALIZE_FIELD(ComponentButton*, _exit_debug);

			SERIALIZE_FIELD(GameObject*, _tp_pos1);
			SERIALIZE_FIELD(GameObject*, _tp_pos2);
			SERIALIZE_FIELD(GameObject*, _tp_pos3);

			bool is_active;
			bool _is_god_mode = false;
			std::vector<math::float3> teleport_positions;
			int teleport_iterator = -1;
			PlayerController* _player_controller;
			bool is_wireframe = false;

		};
	} // namespace Scripting
} // namespace Hachiko