#pragma once

#include <scripting/Script.h>
#include <entities/crystals/Stalagmite.h>

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
	
			void OnStart() override;
			void OnUpdate() override;

			void UpdateStalagmiteState(Stalagmite* stalagmite);

			void FallingStalagmite(Stalagmite* stalagmite);

		private:
			void GenerateStalagmites();

		private:

			float falling_elapsed = 0.0f;
			float cooldown_elapsed = 0.0f;

			SERIALIZE_FIELD(float, _falling_time);
			SERIALIZE_FIELD(float, _falling_cooldown);


			// STALAGMITE
			std::vector<Stalagmite*> _stalagmites{};
		};
	}
}

