#include "scriptingUtil/gameplaypch.h"
#include "PillarCheckpoint.h"
#include "entities/player/PlayerController.h"
#include "constants/Scenes.h"
#include "constants/Sounds.h"
#include "misc/AudioManager.h"


// TODO: This include must go
#include <resources/ResourceAnimation.h>

Hachiko::Scripting::PillarCheckpoint::PillarCheckpoint(GameObject* game_object)
	: Script(game_object, "PillarCheckpoint")
	, _respawn_go(nullptr)
    , _activation_range(5.0f)
{
}

void Hachiko::Scripting::PillarCheckpoint::OnAwake()
{
	_animation = game_object->GetComponent<ComponentAnimation>();
	_restart_position = _respawn_go->GetComponent<ComponentTransform>()->GetGlobalPosition();
	_respawn_go->SetActive(false);
	_animation = game_object->GetComponent<ComponentAnimation>();
	_player = Scenes::GetPlayer();
	_level_manager = Scenes::GetLevelManager()->GetComponent<LevelManager>();
	assert(_player != nullptr);
	assert(_level_manager != nullptr);
	_obstacle = game_object->GetComponentInDescendants<ComponentObstacle>();
	if (_obstacle != nullptr)
	{
		_obstacle->AddObstacle();
	}
	if (_light_go)
	{
		_point_light = game_object->GetComponentInDescendants<ComponentPointLight>();
	}
	if (_point_light)
	{
		_point_light->Disable();
	}
}

void Hachiko::Scripting::PillarCheckpoint::OnUpdate()
{
	if (!_player)	return;

	if (!_used && IsPlayerInRange())
	{
		ActivateCheckpoint();
	}

	if (_used && _animation->IsAnimationStopped())
	{
		_animation->SendTrigger("finishedSpinning");
	}
}

void Hachiko::Scripting::PillarCheckpoint::ActivateCheckpoint()
{
	_used = true;

	if (_level_manager)
	{
		_level_manager->SetRespawnPosition(_restart_position);
	}

	AudioManager* audio_manager = Scenes::GetAudioManager()->GetComponent<AudioManager>();
	if (audio_manager)
	{
		audio_manager->GetAudioSource()->PostEvent(Sounds::CHECKPOINT_ACTIVATE);
	}
	// Save Checkpoint on player variable
	_player->GetComponent<PlayerController>()->_initial_pos = _restart_position;

	if (_point_light)
	{
		_point_light->Enable();
	}

	// Activate animation
	if (!_animation)
	{
		HE_LOG("No animation found. Could not play.");
		return;
	}

	_animation->StartAnimating();
	_animation->SendTrigger("isActive");
}

bool Hachiko::Scripting::PillarCheckpoint::IsPlayerInRange()
{
	float3 player_pos = _player->GetComponent<ComponentTransform>()->GetGlobalPosition();
	float distance_to_player = player_pos.Distance(_respawn_go->GetComponent<ComponentTransform>()->GetGlobalPosition());
	
	return distance_to_player <= _activation_range;
}

