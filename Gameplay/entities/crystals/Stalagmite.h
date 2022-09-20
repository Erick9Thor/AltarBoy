#pragma once

#include <scripting/Script.h>
#include "entities/Stats.h"

namespace Hachiko
{
	namespace Scripting
	{
		enum class StalagmiteState
		{
			INVALID,
			FALLING,
			SPAWN_CRYSTAL,
		};

		class Stalagmite : public Script
		{
			SERIALIZATION_METHODS(false)

		public:
			Stalagmite(GameObject* game_object);
			~Stalagmite() override = default;

			StalagmiteState GetState() {
				return stalagmite_state;
			}

			StalagmiteState GetPreviousState() {
				return _previous_state;
			}

			void SetNewState(StalagmiteState state)
			{
				stalagmite_state = state;
			}

			void SetPreviousState(StalagmiteState state)
			{
				_previous_state = state;
			}

			GameObject* GetGEO() {
				return GEO;
			}

			void Falling();

		private:

			StalagmiteState stalagmite_state = StalagmiteState::INVALID;
			StalagmiteState _previous_state = StalagmiteState::INVALID;

			SERIALIZE_FIELD(GameObject*, _explosion_indicator_helper);
			SERIALIZE_FIELD(GameObject*, _explosion_effect);

			SERIALIZE_FIELD(GameObject*, GEO);
		};
	}
}


