#include "scriptingUtil/gameplaypch.h"

#include "DebugManager.h"
#include "PlayerController.h"
#include "Scenes.h"

#include <Algorithm/Random/LCG.h>
//#include <components/ComponentTransform.h>

Hachiko::Scripting::DebugManager::DebugManager(GameObject* game_object)
	: Script(game_object, "DebugManager")
	, _player(nullptr)
	, _button_back(nullptr)
	, _teleport_next_pos(nullptr)
	, _teleport_prev_pos(nullptr)
	, _teleport_add_pos(nullptr)
	, _add_health(nullptr)
	, _remove_health(nullptr)
	, _increase_max_hp(nullptr)
	, _decrease_max_hp(nullptr)
	, _increase_move_speed(nullptr)
	, _decrease_move_speed(nullptr)
	, _increase_attack_cd(nullptr)
	, _decrease_attack_cd(nullptr)
	, _increase_attack_power(nullptr)
	, _decrease_attack_power(nullptr)
	, _god_mode(nullptr)
	, _spawn_enemy(nullptr)
	, _unlock_skills(nullptr)
	, _toggle_performance_output(nullptr)
	, _performance_menu(nullptr)
	, _toggle_vsync(nullptr)
	, _text_fps(nullptr)
	, _text_ms(nullptr)
	, _toggle_wireframe(nullptr)
	, _toggle_show_colliders(nullptr)
	, _exit_debug(nullptr)
	, _tp_pos1(nullptr)
	, _tp_pos2(nullptr)
	, _tp_pos3(nullptr)
{
}

void Hachiko::Scripting::DebugManager::OnAwake()
{
	is_active = false;
	is_wireframe = false;
	is_performance = false;
	

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
	_player_controller = _player->GetComponent<PlayerController>();
	is_vsync = Debug::GetVsync();

	for (GameObject* child : game_object->children)
	{
		child->SetActive(is_active);
	}

}

void Hachiko::Scripting::DebugManager::OnUpdate()
{
	
	if (Input::IsKeyDown(Input::KeyCode::KEY_F2))
	{
		_player_controller->_isInDebug = !_player_controller->_isInDebug;
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


	_performance_menu->SetActive(is_performance);

	// register button interactions
	HandleButtonInteraction();

	if (is_performance)
	{
		std::string fps =  std::to_string(int(Debug::GetFps()));
		std::string ms = std::to_string(Debug::GetMs());

		_text_fps->SetText(fps.c_str());
		_text_ms->SetText(ms.c_str());
	}

}

void Hachiko::Scripting::DebugManager::HandleButtonInteraction()
{
	if (_button_back->IsSelected())
	{
		HE_LOG("_button_back pressed");
		SceneManagement::SwitchScene(Scenes::MAIN_MENU);
	}

	if (_teleport_next_pos->IsSelected())
	{
		HE_LOG("_teleport_next_pos pressed");
		if (teleport_positions.size() == 0)
		{
			HE_LOG("ERROR: teleport_positions vector is empty");
		}
		else
		{
			++teleport_iterator;
			if (teleport_iterator >= teleport_positions.size())
			{
				teleport_iterator = 0;
			}
			float3 current_position = teleport_positions[teleport_iterator];
			_player->GetTransform()->SetGlobalPosition(current_position);


			HE_LOG("Iterator: %d", teleport_iterator);
		}

	}
	if (_teleport_prev_pos->IsSelected())
	{
		HE_LOG("_teleport_prev_pos pressed");
		if (teleport_positions.size() == 0)
		{
			HE_LOG("ERROR: teleport_positions vector is empty");
		}
		else
		{
			--teleport_iterator;
			if (teleport_iterator < 0)
			{
				teleport_iterator = teleport_positions.size() - 1;
			}
			float3 current_position = teleport_positions[teleport_iterator];
			_player->GetTransform()->SetGlobalPosition(current_position);

			HE_LOG("Iterator: %d", teleport_iterator);
		}

	}
	if (_teleport_add_pos->IsSelected())
	{
		teleport_positions.push_back(_player->GetTransform()->GetGlobalPosition());
	}

	// Edit Character Stats
	
	if (_add_health->IsSelected())
	{
		if (_player_controller->_combat_stats->_current_hp < _player_controller->_combat_stats->_max_hp)
		{
			_player_controller->_combat_stats->_current_hp += 1;
			HE_LOG("Health now %i", _player_controller->_combat_stats->_current_hp);
		}
		else
		{
			HE_LOG("Max health reached %u", _player_controller->_combat_stats->_current_hp);
		}
	}
	if (_remove_health->IsSelected())
	{
		_player_controller->_combat_stats->_current_hp -= 1;
		HE_LOG("Health now %i", _player_controller->_combat_stats->_current_hp);
	}
	/*
	if (_increase_max_hp->IsSelected())
	{
		_player_controller->_stats._max_hp += 1;
		HE_LOG("Max health now %d", _player_controller->_stats._max_hp);
	}
	if (_decrease_max_hp->IsSelected())
	{
		_player_controller->_stats._max_hp -= 1;
		HE_LOG("Max health now %d", _player_controller->_stats._max_hp);
	}
	*/
	
	if (_increase_move_speed->IsSelected())
	{
		_player_controller->_combat_stats->_move_speed += 1.0f;
		HE_LOG("Move speed now %f", _player_controller->_combat_stats->_move_speed);
	}
	if (_decrease_move_speed->IsSelected())
	{
		_player_controller->_combat_stats->_move_speed -= 1.0f;
		HE_LOG("Move speed now %f", _player_controller->_combat_stats->_move_speed);
	}
	/*
	if (_increase_attack_cd->IsSelected())
	{
		_player_controller->_stats._attack_cd += 0.1f;
		HE_LOG("Attack speed now %f", _player_controller->_stats._attack_cd);
	}
	
	if (_decrease_attack_cd->IsSelected())
	{
		_player_controller->_stats._attack_cd -= 0.1f;
		HE_LOG("Attack speed now %f", _player_controller->_stats._attack_cd);
	}
	*/
	if (_increase_attack_power->IsSelected())
	{
		_player_controller->_combat_stats->_attack_power += 1;
		HE_LOG("Attack power now %d", _player_controller->_combat_stats->_attack_power);
	}
	if (_decrease_attack_power->IsSelected())
	{
		_player_controller->_combat_stats->_attack_power -= 1;
		HE_LOG("Attack power now %d", _player_controller->_combat_stats->_attack_power);
	}
	
	// Player States
	if (_god_mode->IsSelected())
	{
		_player_controller->_god_mode = !_player_controller->_god_mode;
	}
	/*

	if (_unlock_skills->IsSelected())
	{
		HE_LOG("_toggle_invulnerable pressed");
	}
	*/

	if (_spawn_enemy->IsSelected())
	{
		HE_LOG("_spawn_enemy pressed");
		UID* enemy_uid = new UID(11363594999076676195);
		GameObject* enemy = GameObject::Instantiate(enemy_uid, game_object->parent->FindDescendantWithName("Enemies"));
		float3 player_pos = _player->GetTransform()->GetGlobalPosition();
		enemy->GetTransform()->SetGlobalPosition(player_pos + float3(-3, 0, 0));
	}

	// NOTE: This nullptr check is just for experiment purposes on the new exposed component text. 
	// Marius will probably gonna set this button from editor and save it so it will be never 
	// nullptr in the future.
	// TODO: Delete this when the previous note is addressed.
	if (_toggle_performance_output != nullptr)
	{
		if (_toggle_performance_output->IsSelected())
		{
			is_performance = !is_performance;
		}

		if (_toggle_vsync->IsSelected())
		{
			is_vsync = !is_vsync;
			Debug::SetVsync(is_vsync);
		}

		if (_toggle_show_colliders->IsSelected())
		{
			HE_LOG("_toggle_show_colliders pressed");
		}

		if (_toggle_wireframe->IsSelected())
		{
			is_wireframe = !is_wireframe;
			// Set polygon mode to GL_FILL if is_wireframe is false, GL_LINE if true:
			Debug::SetPolygonMode(!is_wireframe);
		}

		if (_exit_debug->IsSelected())
		{
			is_active = !is_active;
			for (GameObject* child : game_object->children)
			{
				_player_controller->_isInDebug = !_player_controller->_isInDebug;
				child->SetActive(is_active);
			}
		}
	}
}