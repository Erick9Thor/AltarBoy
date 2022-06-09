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
			void OnStart() override;
			void OnUpdate() override;
		private:

			ComponentAudioSource* _audio_source;

			bool isPlayingMoving1 = false;
			bool isPlayingMoving = false;

			GameObject* enemy_pool;

			SERIALIZE_FIELD(std::vector<GameObject*>, enemics);
			int cont;
		};
	} // namespace Scripting
} // namespace Hachiko


