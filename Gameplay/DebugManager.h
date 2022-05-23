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
			bool is_active;
			bool _is_god_mode = false;
		};
	} // namespace Scripting
} // namespace Hachiko