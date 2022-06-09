#pragma once
#include <scripting/Script.h>

namespace Hachiko
{

	class GameObject;
	class ComponentAudioSource;

	namespace Scripting
	{

		class AudioManager : public Script
		{

			SERIALIZATION_METHODS(false)

		public:
			AudioManager(Hachiko::GameObject* game_object);
			~AudioManager() override = default;

			void OnAwake() override;
			void OnUpdate() override;
		private:

			ComponentAudioSource* _audio_source;

			bool isPlayingMoving = false;

			GameObject* enemy_pool;
		};
	} // namespace Scripting
} // namespace Hachiko


