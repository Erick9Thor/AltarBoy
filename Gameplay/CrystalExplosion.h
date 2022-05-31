#pragma once

#include <scripting/Script.h>
#include "Stats.h"

namespace Hachiko
{
	class GameObject;
	class ComponentTransform;

	namespace Scripting
	{
		class CrystalExplosion : public Script
		{
			SERIALIZATION_METHODS(false)


		public:
			CrystalExplosion(GameObject* game_object);
			~CrystalExplosion() override = default;


			void OnAwake() override;
			void OnStart() override;
			void OnUpdate() override;

			void CheckRadiusExplosion();

			void ReceiveDamage(int damage, float3 direction);

		private:
			void DestroyCristall();

		public:
			SERIALIZE_FIELD(Stats, _stats);

		private:
			ComponentTransform* transform;
			SERIALIZE_FIELD(GameObject*, _player);

			SERIALIZE_FIELD(GameObject*, _explosion_crystal);
			SERIALIZE_FIELD(GameObject*, _static_crystal);

			SERIALIZE_FIELD(unsigned, _crashing_index);
			SERIALIZE_FIELD(float, _explosion_radius);
			SERIALIZE_FIELD(bool, _explosive_crystal);


			math::float3 _player_pos;

			GameObject* enemies;
		};
	}
}