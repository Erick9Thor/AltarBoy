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
			SERIALIZE_FIELD(ComponentButton*, _button_back);
			SERIALIZE_FIELD(ComponentButton*, _teleport_next_pos);
			SERIALIZE_FIELD(ComponentButton*, _add_health);
			SERIALIZE_FIELD(ComponentButton*, _remove_health);
			SERIALIZE_FIELD(ComponentButton*, _toggle_invulnerable);
			// placeholder for _navigate_in_godmode
			SERIALIZE_FIELD(ComponentButton*, _spawn_enemy);
			SERIALIZE_FIELD(ComponentButton*, _toggle_performance_output);
			SERIALIZE_FIELD(ComponentButton*, _toggle_wireframe);
			SERIALIZE_FIELD(ComponentButton*, _toggle_show_colliders);
			bool is_active;
			bool _is_god_mode = false;
		};
	} // namespace Scripting
} // namespace Hachiko