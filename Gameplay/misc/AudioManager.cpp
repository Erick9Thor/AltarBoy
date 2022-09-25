#include "scriptingUtil/gameplaypch.h"
#include "AudioManager.h"

#include "modules/ModuleSceneManager.h"

#include "constants/Sounds.h"

#include "entities/enemies/EnemyController.h"

Hachiko::Scripting::AudioManager::AudioManager(GameObject* game_object)
	: Script(game_object, "AudioManager")
	, _enemies_in_combat(0)
	, _in_combat(false)
	, _in_gaunlet(false)
{
}

void Hachiko::Scripting::AudioManager::OnAwake()
{
	enemy_pool = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Enemies");
	_audio_source = game_object->GetComponent<ComponentAudioSource>();
	SetNavigation();
}

void Hachiko::Scripting::AudioManager::OnStart()
{
	SetNavigation();
	_audio_source->PostEvent(Sounds::PLAY_NAVIGATION);
	_audio_source->PostEvent(Sounds::PLAY_WIND);
	_audio_source->PostEvent(Sounds::PLAY_PEBBLE);
	updated = true;
}

void Hachiko::Scripting::AudioManager::OnUpdate()
{
	UpdateState();
}

void Hachiko::Scripting::AudioManager::UpdateState()
{
	if (updated)
	{
		return;
	}

	if (_in_gaunlet || _in_combat)
	{
		SetCombat();
	}
	else
	{
		SetNavigation();
	}

	updated = true;
}

void Hachiko::Scripting::AudioManager::RegisterCombat()
{
	++_enemies_in_combat;
	if (!_in_combat)
	{
		_in_combat = true;
		updated = false;
	}
}

void Hachiko::Scripting::AudioManager::UnregisterCombat()
{
	--_enemies_in_combat;
	if (_enemies_in_combat <= 0)
	{
		_enemies_in_combat = 0;
		updated = false;
		_in_combat = false;
	}
}

void Hachiko::Scripting::AudioManager::RegisterGaunlet()
{
	_in_gaunlet = true;
	_in_combat = true;
	updated = false;
}

void Hachiko::Scripting::AudioManager::UnregisterGaunlet()
{
	_in_gaunlet = false;
	_in_combat = false;
	updated = false;
}

void Hachiko::Scripting::AudioManager::Restart()
{
	OnStart();
}

void Hachiko::Scripting::AudioManager::SetCombat()
{
	_audio_source->SetRTPCValue(Sounds::ENEMY_AWARE, 100);
}

void Hachiko::Scripting::AudioManager::SetNavigation()
{
	_audio_source->SetRTPCValue(Sounds::ENEMY_AWARE, 0);
}

void Hachiko::Scripting::AudioManager::StopMusic()
{
	//_audio_source->PostEvent(Sounds::STOP_COMBAT);
	//_audio_source->PostEvent(Sounds::STOP_NAVIGATION);
}
