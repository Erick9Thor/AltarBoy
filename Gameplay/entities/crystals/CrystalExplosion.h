#pragma once

#include <scripting/Script.h>
#include "entities/Stats.h"

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
			void ExplodeCrystal();

			void RegisterHit(int damage);
			bool isAlive() { return _stats->IsAlive(); };

		private:
			void DestroyCrystal();

		public:
			Stats* _stats;

		private:
			ComponentTransform* transform;
			SERIALIZE_FIELD(GameObject*, _player);

			SERIALIZE_FIELD(GameObject*, _explosion_crystal);
			SERIALIZE_FIELD(GameObject*, _static_crystal);

			SERIALIZE_FIELD(unsigned, _crashing_index);
			SERIALIZE_FIELD(float, _detecting_radius);
			SERIALIZE_FIELD(float, _explosion_radius);
			SERIALIZE_FIELD(bool, _explosive_crystal);

			ComponentAudioSource* _audio_source;
			bool is_destroyed = false;

			math::float3 _player_pos;

			GameObject* enemies;
		};
	}
}