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

void Hachiko::Scripting::AudioManager::OnStart()
{
	_audio_source->PostEvent(Sounds::PLAY_NAVIGATION);
	_audio_source->PostEvent(Sounds::PLAY_WIND);
	_audio_source->PostEvent(Sounds::PLAY_PEBBLE);
}

void Hachiko::Scripting::AudioManager::OnUpdate()
{
	// Sorry :_C
	//if (enemy_pool && enemy_pool->children.size() > 0) 
	//{
	//	for (unsigned int i = 0; i < enemy_pool->children.size(); ++i)
	//	{
	//		BugState current_state = enemy_pool->children[i]->GetComponent<EnemyController>()->GetState();
	//		if (current_state == BugState::MOVING && !isPlayingMoving1)
	//		{
	//			//enemics.push_back(enemy_pool->children[i]);
	//			cont++;
	//			bool found = false;
	//			unsigned long long iu = enemy_pool->children[i]->GetID();
	//			for (GameObject en : enemics)
	//			{
	//				unsigned long long iuiu = en.GetID();
	//				if (iuiu == iu)
	//				{
	//					found = true;
	//					break;
	//				}
	//			}
	//			if (!found) 
	//			{
	//				enemics.push_back(enemy_pool->children[i]);
	//			}
	//			isPlayingMoving1 = true;
	//			/*_audio_source->PostEvent(Sounds::PLAY_COMBAT);
	//			_audio_source->PostEvent(Sounds::STOP_NAVIGATION);*/
	//		} else if (current_state == BugState::MOVING_BACK || current_state == BugState::DEAD && isPlayingMoving1)
	//		{
	//			cont--;
	//			
	//			unsigned long long iu = enemy_pool->children[i]->GetID();
	//			for (unsigned int j = 0; j < enemics.size(); ++j)
	//			{
	//				unsigned long long iuiu = enemy_pool->children[j]->GetID();
	//				if (iuiu == iu)
	//				{
	//					enemics.erase(enemics.begin() + j);
	//					enemics.resize(enemics.size() - 1);
	//				}
	//			}

	//			//enemics.pop_back();
	//			isPlayingMoving1 = false;
	//			/*_audio_source->PostEvent(Sounds::PLAY_NAVIGATION);
	//			_audio_source->PostEvent(Sounds::STOP_COMBAT);*/
	//		}
	//	}
	//}
	//if (enemics.size() <= 0 && isPlayingMoving)
	//{
	//	isPlayingMoving = false;
	//	_audio_source->PostEvent(Sounds::PLAY_NAVIGATION);
	//	_audio_source->PostEvent(Sounds::STOP_COMBAT);
	//}
	//else if(enemics.size() > 0 && !isPlayingMoving)
	//{
	//	isPlayingMoving = true;
	//	_audio_source->PostEvent(Sounds::PLAY_COMBAT);
	//	_audio_source->PostEvent(Sounds::STOP_NAVIGATION);
	//}
	//return;
}

