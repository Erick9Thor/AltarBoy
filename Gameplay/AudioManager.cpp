#include "scriptingUtil/gameplaypch.h"
#include "AudioManager.h"

#include "modules/ModuleSceneManager.h"

#include "Sounds.h"

#include <EnemyController.h>



Hachiko::Scripting::AudioManager::AudioManager(GameObject* game_object)
	: Script(game_object, "AudioManager")
{
}

void Hachiko::Scripting::AudioManager::OnAwake()
{
	enemy_pool = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Enemies");
	_audio_source = game_object->GetComponent<ComponentAudioSource>();
}

void Hachiko::Scripting::AudioManager::OnUpdate()
{
	if (enemy_pool && enemy_pool->children.size() > 0) 
	{
		for (unsigned int i = 0; i < enemy_pool->children.size(); ++i)
		{
			BugState current_state = enemy_pool->children[i]->GetComponent<EnemyController>()->GetState();
			if (current_state == BugState::MOVING && !isPlayingMoving)
			{
				isPlayingMoving = true;
				_audio_source->PostEvent(Sounds::PLAY_COMBAT);
				_audio_source->PostEvent(Sounds::STOP_NAVIGATION);
			}
		}
	}
	return;
}

