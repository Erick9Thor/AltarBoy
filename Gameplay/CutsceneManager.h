#pragma once
#include <scripting\Script.h>
namespace Hachiko
{
	class ComponentVideo;

	namespace Scripting
	{
		class CutsceneManager : public Script
		{
		public:
			SERIALIZATION_METHODS(false)
			CutsceneManager(GameObject* game_object);

			void OnAwake() override;
			void OnStart() override;
			void OnUpdate() override;
		private:
			SERIALIZE_FIELD(GameObject*, _cutscene);
			ComponentVideo* _cutscene_video = nullptr;
		};
	}
}

