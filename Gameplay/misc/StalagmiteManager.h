#pragma once

#include <scripting/Script.h>
#include <entities/crystals/Stalagmite.h>
#include <entities/player/PlayerCamera.h>

namespace Hachiko
{
	namespace Scripting
	{
		class StalagmiteManager : public Script
		{
			SERIALIZATION_METHODS(false)

		public:
			StalagmiteManager(GameObject* game_object);
			~StalagmiteManager() override = default;
	
			void OnAwake() override;
			void OnStart() override;
			void OnUpdate() override;

			void UpdateStalagmiteState(Stalagmite* stalagmite);

			void FallingStalagmite(Stalagmite* stalagmite);

		private:
			void GenerateStalagmites();

		private:

			float falling_elapsed = 0.0f;
			float cooldown_elapsed = 0.0f;
			float _falling_time = 0.5f;

			PlayerCamera* _player_camera = nullptr;

			SERIALIZE_FIELD(float, _falling_cooldown);
			SERIALIZE_FIELD(GameObject*, _crystal_parent);

			// STALAGMITE
			std::vector<Stalagmite*> _stalagmites{};
		};
	}
}

