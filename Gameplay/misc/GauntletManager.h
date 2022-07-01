#pragma once

#include <scripting/Script.h>
#include "entities/player/CombatManager.h"
#include "misc/DoorController.h"

namespace Hachiko
{
	class GameObject;
	class ComponentTransform;
	namespace Scripting
	{
		class GauntletManager : public Script
		{
			SERIALIZATION_METHODS(false)
			
		public:
			GauntletManager(GameObject* game_object);
			~GauntletManager() override = default;

			void OnAwake() override;
			void OnStart() override;
			void OnUpdate() override;

			SERIALIZE_FIELD(GameObject*, _combat_manager_go);
			SERIALIZE_FIELD(float, _trigger_radius);
			SERIALIZE_FIELD(bool, _use_trigger);

			SERIALIZE_FIELD(GameObject*, _door_1);
			SERIALIZE_FIELD(GameObject*, _door_2);

			SERIALIZE_FIELD(GameObject*, _pack_1);
			SERIALIZE_FIELD(GameObject*, _pack_2);
			SERIALIZE_FIELD(GameObject*, _pack_3);		

		private:

			void StartGauntlet();
			void ResetGauntlet();
			void CheckRoundStatus();
			void OpenDoors();
			void CloseDoors();
			void SpawnRound(unsigned round);

			unsigned current_round = 0;
			bool completed = false;
			bool started = false;
			CombatManager* _combat_manager = nullptr;
			std::vector<GameObject*> _enemy_packs{};
			std::vector<DoorController*> _doors{};

		};
	}

}



