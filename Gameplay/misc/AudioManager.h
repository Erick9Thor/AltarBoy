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

			void RegisterCombat();
			void UnregisterCombat();

			void RegisterGaunlet();
			void UnregisterGaunlet();

			void Restart();
		private:
			SERIALIZE_FIELD(int, _enemies_in_combat);
			SERIALIZE_FIELD(bool, _in_combat);
			SERIALIZE_FIELD(bool, _in_gaunlet);
			SERIALIZE_FIELD(std::vector<GameObject*>, enemics);

			bool updated = false;
			GameObject* enemy_pool = nullptr;
			ComponentAudioSource* _audio_source = nullptr;

			void StopMusic();
			void UpdateState();
			void SetCombat();
			void SetNavigation();
		};
	} // namespace Scripting
} // namespace Hachiko


