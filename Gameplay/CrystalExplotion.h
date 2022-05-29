#pragma once

#include <scripting/Script.h>
#include "Stats.h"

namespace Hachiko
{
	class GameObject;
	class ComponentTransform;

	namespace Scripting
	{
		class CrystalExplotion : public Script
		{
			SERIALIZATION_METHODS(false)


		public:
			CrystalExplotion(GameObject* game_object);
			~CrystalExplotion() override = default;


			void OnAwake() override;
			void OnStart() override;
			void OnUpdate() override;

			void CheckRadiusExplotion();

			void ReceiveDamage(int damage, float3 direction);

		private:
			void DestroyCristall();

		public:
			SERIALIZE_FIELD(Stats, _stats);

		private:
			ComponentTransform* transform;
			SERIALIZE_FIELD(GameObject*, _player);

			SERIALIZE_FIELD(GameObject*, _explotion_crystal);
			SERIALIZE_FIELD(GameObject*, _static_crystal);

			SERIALIZE_FIELD(unsigned, _crashing_index);
			SERIALIZE_FIELD(float, _explotion_radius);
			SERIALIZE_FIELD(bool, _explosive_crystal);


			math::float3 _player_pos;

			GameObject* enemies;
		};
	}
}