#pragma once

#include <scripting/Script.h>
#include "Stats.h"

namespace Hachiko
{
	class GameObject;
	class ComponentTransform;

	namespace Scripting
	{
		enum class CristalState
		{
			IDLE,
			CRASHING
		};

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
			SERIALIZE_FIELD(ComponentAnimation*, _animator);
			SERIALIZE_FIELD(CristalState, _state);
			SERIALIZE_FIELD(unsigned, _crashing_index);
			SERIALIZE_FIELD(float, _explotion_radius);
			SERIALIZE_FIELD(bool, _explosive_crystal);

			float3 _knockback_pos = float3::zero;

			math::float3 _player_pos;
			math::float3 _crystal_pos;

			GameObject* enemies;
		};
	}
}