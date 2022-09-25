#include "scriptingUtil/gameplaypch.h"

#include "misc/TimeManager.h"
#include "constants/Scenes.h"
#include "entities/player/PlayerController.h"
#include "entities/enemies/EnemyController.h"

#include "Tutorial.h"

Hachiko::Scripting::Tutorial::Tutorial(GameObject* game_object)
	: Script(game_object, "Tutorial")
{
}

void Hachiko::Scripting::Tutorial::OnAwake()
{
	_time_manager = game_object->GetComponent<TimeManager>();

	_player = Scenes::GetPlayer();
	if (_player)
	{
		_player_controller = _player->GetComponent<PlayerController>();
	}

	if (_first_enemy != nullptr)
	{
		_enemy_controller = _first_enemy->GetComponent<EnemyController>();
	}

	_player_tutorial = game_object->children[0];
	_parasite_tutorial = game_object->children[1];
	_player_tutorial->SetActive(false);
	_parasite_tutorial->SetActive(false);
}

void Hachiko::Scripting::Tutorial::OnStart()
{
	_player_tutorial->SetActive(true);
	BlockActions();
}

void Hachiko::Scripting::Tutorial::OnUpdate()
{
	if (Input::IsKeyDown(Input::KeyCode::KEY_T))
	{
		_player_tutorial->SetActive(true);
		BlockActions();
	}

	if (Input::IsKeyDown(Input::KeyCode::KEY_Y))
	{
		_player_tutorial->SetActive(false);
		_parasite_tutorial->SetActive(false);
		UnblockActions();
	}

	if (!_tutorial_showed && _enemy_controller->ParasiteDropped())
	{
		_parasite_tutorial->SetActive(true);
		BlockActions();
		_tutorial_showed = true;
	}
}

void Hachiko::Scripting::Tutorial::BlockActions()
{
	Time::SetTimeScale(0.0001f);
	_player_controller->SetLockTime(60.0f);
}

void Hachiko::Scripting::Tutorial::UnblockActions()
{
	_player_controller->SetLockTime(0.0f);
	_time_manager->InterpolateToTimeScale(1.0f, 0.1f, 0.2f);
}
