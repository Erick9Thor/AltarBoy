#include "scriptingUtil/gameplaypch.h"
#include "DebugManager.h"
#include "PlayerController.h"

#include <components/ComponentTransform.h>
#include <core/GameObject.h>
#include "Scenes.h"

Hachiko::Scripting::DebugManager::DebugManager(GameObject* game_object)
	: Script(game_object, "DebugManager")
	, _button_back(nullptr)
	, _teleport_next_pos(nullptr)
	, _teleport_prev_pos(nullptr)
	, _teleport_add_pos(nullptr)
	, _add_health(nullptr)
	, _remove_health(nullptr)
	, _god_mode(nullptr)
	, _flying_mode(nullptr)
	, _spawn_enemy(nullptr)
	, _unlock_skills(nullptr)
	, _toggle_performance_output(nullptr)
	, _toggle_wireframe(nullptr)
	, _toggle_show_colliders(nullptr)
	, _player(nullptr)
	, _exit_debug(nullptr)
	, _tp_pos1(nullptr)
	, _tp_pos2(nullptr)
	, _tp_pos3(nullptr)
{
}

void Hachiko::Scripting::DebugManager::OnAwake()
{
	is_active = false;
	_is_god_mode = false;

	if (_tp_pos1) 
	{
		teleport_positions.push_back(_tp_pos1->GetTransform()->GetGlobalPosition());
	}
	if (_tp_pos2)
	{
		teleport_positions.push_back(_tp_pos2->GetTransform()->GetGlobalPosition());
	}
	if (_tp_pos3)
	{
		teleport_positions.push_back(_tp_pos3->GetTransform()->GetGlobalPosition());
	}

}

void Hachiko::Scripting::DebugManager::OnStart()
{
	_player = game_object->parent->GetFirstChildWithName("PlayerC");
	_player_controller = _player->GetComponent<PlayerController>();

	for (GameObject* child : game_object->children)
	{
		child->SetActive(is_active);
	}
}

void Hachiko::Scripting::DebugManager::OnUpdate()
{
	
	if (Input::IsKeyDown(Input::KeyCode::KEY_F3))
	{
		_player_controller->_isInDebug = !_player_controller->_isInDebug;
		HE_LOG("HOLAA");
		is_active = !is_active;
		for (GameObject* child : game_object->children)
		{
			child->SetActive(is_active);
		}
	}
	if(!is_active)
	{
		return;
	}


	// register button interactions

	if (_button_back->IsSelected())
	{
		HE_LOG("_button_back pressed");
		SceneManagement::SwitchScene(Scenes::MAIN_MENU);
	}
	
	if (_teleport_next_pos->IsSelected())
	{
		if (teleport_positions.size() == 0)
		{
			HE_LOG("ERROR: teleport_positions vector is empty");
		}
		else
		{
			float3 current_position = teleport_positions[teleport_iterator];
			_player->GetTransform()->SetGlobalPosition(current_position);
		}

	}
	if (_teleport_prev_pos->IsSelected())
	{
		if (teleport_positions.size() == 0)
		{
			HE_LOG("ERROR: teleport_positions vector is empty");
		}
		else
		{
			float3 current_position = teleport_positions[teleport_iterator];
			_player->GetTransform()->SetGlobalPosition(current_position);
		}

	}
	if (_teleport_add_pos->IsSelected())
	{
		HE_LOG("_teleport_add_pos pressed");
		teleport_positions.push_back(_player->GetTransform()->GetGlobalPosition());
	}
	if (_add_health->IsSelected())
	{
		_player_controller->_stats._current_hp += 1;
		HE_LOG("Health now %d", _player_controller->_stats._current_hp);
	}
	if (_remove_health->IsSelected())
	{
		_player_controller->_stats._current_hp -= 1;
		HE_LOG("Health now %d", _player_controller->_stats._current_hp);
	}
	if (_god_mode->IsSelected())
	{
		HE_LOG("_god_mode pressed");
	}
	if (_flying_mode->IsSelected())
	{
		HE_LOG("_flying_mode pressed");
	}
	if (_spawn_enemy->IsSelected())
	{
		HE_LOG("_spawn_enemy pressed");
	}
	if (_unlock_skills->IsSelected())
	{
		HE_LOG("_toggle_invulnerable pressed");
	}
	if (_toggle_performance_output->IsSelected())
	{
		HE_LOG("_toggle_performance_output pressed");
	}
	if (_toggle_wireframe->IsSelected())
	{
		HE_LOG("_toggle_wireframe pressed");
	}
	if (_toggle_show_colliders->IsSelected())
	{
		HE_LOG("_toggle_show_colliders pressed");
	}
	if (_exit_debug->IsSelected())
	{
		is_active = !is_active;
		for (GameObject* child : game_object->children)
		{
			child->SetActive(is_active);
		}
	}
	
}
