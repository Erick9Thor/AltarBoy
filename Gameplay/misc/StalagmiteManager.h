#pragma once

#include <scripting/Script.h>

namespace Hachiko
{
	namespace Scripting
	{

		enum class StalagmiteState
		{
			INVALID,
			FALLING_AREA,
			COLLAPSE,
			SPAWN_CRYSTAL,
		};

		struct Stalagmite
		{
			StalagmiteState stalagmite_state = StalagmiteState::INVALID;
			StalagmiteState _previous_state = StalagmiteState::INVALID;
			unsigned damage = 1.f;
			float knockback_distance = 0.f;
			float width = 0.f;
			float range = 0.f;
		};

		class StalagmiteManager : public Script
		{
			SERIALIZATION_METHODS(false)

		public:
			StalagmiteManager(GameObject* game_object);
			~StalagmiteManager() override = default;
	
			void OnStart() override;
			void OnUpdate() override;

			void UpdateStalagmiteState(Stalagmite* stalagmite);


		private:
			void GenerateStalagmites();


		private:
			// SPAWN control
			Quat _spawn_rot;

			math::float3 _player_pos;
			math::float3 _target_pos;
			math::float3 _current_pos;

			SERIALIZE_FIELD(float, _spawn_cooldown);

			// STALAGMITE
			SERIALIZE_FIELD(GameObject*, _go_stalagmite);
			std::vector<Stalagmite*> _stalagmites{};

		};
	}
}

