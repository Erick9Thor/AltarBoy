#include "scriptingUtil/gameplaypch.h"
#include "AudioManager.h"

#include "modules/ModuleSceneManager.h"

#include "constants/Sounds.h"

#include "entities/enemies/EnemyController.h"

Hachiko::Scripting::AudioManager::AudioManager(GameObject* game_object)
	: Script(game_object, "AudioManager")
	, _enemies_in_combat(0)
	, _previous_in_combat(false)
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
	bool current_in_combat = _enemies_in_combat > 0;

	if (_previous_in_combat == current_in_combat)
	{
		return;
	}

	_previous_in_combat = current_in_combat;

	if (!current_in_combat)
	{
		_audio_source->PostEvent(Sounds::STOP_COMBAT);
		_audio_source->PostEvent(Sounds::PLAY_NAVIGATION);
	}
	else
	{
		_audio_source->PostEvent(Sounds::PLAY_COMBAT);
		_audio_source->PostEvent(Sounds::STOP_NAVIGATION);
	}
}

void Hachiko::Scripting::AudioManager::RegisterCombat()
{
	++_enemies_in_combat;
}

void Hachiko::Scripting::AudioManager::UnregisterCombat()
{
	--_enemies_in_combat;

	_enemies_in_combat = _enemies_in_combat < 0 ? 0 : _enemies_in_combat;
}
