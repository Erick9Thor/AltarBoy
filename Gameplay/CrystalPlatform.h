#pragma once

#include <scripting/Script.h>
#include "entities/Stats.h"

namespace Hachiko
{
	class GameObject;
	class ComponentTransform;

	namespace Scripting
	{
		enum class PlatformState
		{
			INVALID,
			IDLE,
			PLATFORM,
			SHAKING
		};

		class CrystalPlatform : public Script
		{
			SERIALIZATION_METHODS(false)

		public:
			CrystalPlatform(GameObject* game_object);
			~CrystalPlatform() override = default;

			void OnAwake() override;
			void OnStart() override;
			void OnUpdate() override;

			void RegisterHit(int damage);

			PlatformState GetState() {
				return _state;
			}

		private:
			void ShowPlatform();
			void RegenerateCrystal();
			void UpdatePlatformStatus();
			void Shaking();

			float3 Shake();
			void Shake(float time, float intensity);


		public:
			Stats* _stats;
			PlatformState _state = PlatformState::INVALID;

			bool is_platform_active = false;
			bool is_shaking = false;

			ComponentAnimation* exploding_platform;
			ComponentObstacle* obstacle;

			SERIALIZE_FIELD(float, _seconds_before_shaking);
			SERIALIZE_FIELD(float, _seconds_shaking);
			SERIALIZE_FIELD(PlatformState, _previous_state);

			SERIALIZE_FIELD(GameObject*, _invisible_obstacle);
			SERIALIZE_FIELD(GameObject*, _crystal_explotion);

			float shake_elapsed;
			float shake_time;
			float shake_intensity;
			float shake_magnitude;
		};
	}
}