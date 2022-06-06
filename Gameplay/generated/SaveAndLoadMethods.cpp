#include "scriptingUtil/gameplaypch.h"
#include <yaml-cpp/yaml.h>
#include <core/serialization/TypeConverter.h>
#include "BackToMainMenu.h"
#include "BugAnimationManager.h"
#include "CrystalExplosion.h"
#include "DynamicCamera.h"
#include "EnemyController.h"
#include "FancyLights.h"
#include "MainMenuManager.h"
#include "PlayerAnimationManager.h"
#include "PlayerCamera.h"
#include "PlayerController.h"
#include "PlayerSoundManager.h"



void Hachiko::Scripting::BackToMainMenu::OnSave(YAML::Node& node) const
{
	if (_button_back != nullptr && _button_back->GetGameObject() != nullptr)
	{
		node["'_button_back@ComponentButton*'"] = _button_back->GetGameObject()->GetID();
	}
	else
	{
		node["'_button_back@ComponentButton*'"] = 0;
	}
}

void Hachiko::Scripting::BackToMainMenu::OnLoad()
{
	if (load_node["'_button_back@ComponentButton*'"].IsDefined())
	{
		GameObject* _button_back_owner__temp = SceneManagement::FindInCurrentScene(load_node["'_button_back@ComponentButton*'"].as<unsigned long long>());
		if (_button_back_owner__temp != nullptr)
		{
			_button_back = _button_back_owner__temp->GetComponent<ComponentButton>();
		}
	}
}

void Hachiko::Scripting::BugAnimationManager::OnSave(YAML::Node& node) const
{
	if (_animator != nullptr && _animator->GetGameObject() != nullptr)
	{
		node["'_animator@ComponentAnimation*'"] = _animator->GetGameObject()->GetID();
	}
	else
	{
		node["'_animator@ComponentAnimation*'"] = 0;
	}

	node["'_state_string@std::string'"] = _state_string;

	node["'_idle_index@unsigned'"] = _idle_index;

	node["'_attacking_index@unsigned'"] = _attacking_index;
}

void Hachiko::Scripting::BugAnimationManager::OnLoad()
{
	if (load_node["'_animator@ComponentAnimation*'"].IsDefined())
	{
		GameObject* _animator_owner__temp = SceneManagement::FindInCurrentScene(load_node["'_animator@ComponentAnimation*'"].as<unsigned long long>());
		if (_animator_owner__temp != nullptr)
		{
			_animator = _animator_owner__temp->GetComponent<ComponentAnimation>();
		}
	}

	if (load_node["'_state_string@std::string'"].IsDefined())
	{
		_state_string = load_node["'_state_string@std::string'"].as<std::string>();
	}

	if (load_node["'_idle_index@unsigned'"].IsDefined())
	{
		_idle_index = load_node["'_idle_index@unsigned'"].as<unsigned>();
	}

	if (load_node["'_attacking_index@unsigned'"].IsDefined())
	{
		_attacking_index = load_node["'_attacking_index@unsigned'"].as<unsigned>();
	}
}

void Hachiko::Scripting::CrystalExplosion::OnSave(YAML::Node& node) const
{

	if (_player != nullptr)
	{
		node["'_player@GameObject*'"] = _player->GetID();
	}
	else
	{
		node["'_player@GameObject*'"] = 0;
	}

	if (_explosion_crystal != nullptr)
	{
		node["'_explosion_crystal@GameObject*'"] = _explosion_crystal->GetID();
	}
	else
	{
		node["'_explosion_crystal@GameObject*'"] = 0;
	}

	if (_static_crystal != nullptr)
	{
		node["'_static_crystal@GameObject*'"] = _static_crystal->GetID();
	}
	else
	{
		node["'_static_crystal@GameObject*'"] = 0;
	}

	node["'_crashing_index@unsigned'"] = _crashing_index;

	node["'_detecting_radius@float'"] = _detecting_radius;

	node["'_explosion_radius@float'"] = _explosion_radius;

	node["'_explosive_crystal@bool'"] = _explosive_crystal;
}

void Hachiko::Scripting::CrystalExplosion::OnLoad()
{

	if (load_node["'_player@GameObject*'"].IsDefined())
	{
		_player = SceneManagement::FindInCurrentScene(load_node["'_player@GameObject*'"].as<unsigned long long>());
	}

	if (load_node["'_explosion_crystal@GameObject*'"].IsDefined())
	{
		_explosion_crystal = SceneManagement::FindInCurrentScene(load_node["'_explosion_crystal@GameObject*'"].as<unsigned long long>());
	}

	if (load_node["'_static_crystal@GameObject*'"].IsDefined())
	{
		_static_crystal = SceneManagement::FindInCurrentScene(load_node["'_static_crystal@GameObject*'"].as<unsigned long long>());
	}

	if (load_node["'_crashing_index@unsigned'"].IsDefined())
	{
		_crashing_index = load_node["'_crashing_index@unsigned'"].as<unsigned>();
	}

	if (load_node["'_detecting_radius@float'"].IsDefined())
	{
		_detecting_radius = load_node["'_detecting_radius@float'"].as<float>();
	}

	if (load_node["'_explosion_radius@float'"].IsDefined())
	{
		_explosion_radius = load_node["'_explosion_radius@float'"].as<float>();
	}

	if (load_node["'_explosive_crystal@bool'"].IsDefined())
	{
		_explosive_crystal = load_node["'_explosive_crystal@bool'"].as<bool>();
	}
}

void Hachiko::Scripting::DynamicCamera::OnSave(YAML::Node& node) const
{
	node["'_start_point@math::float3'"] = _start_point;

	node["'_end_point@math::float3'"] = _end_point;

	node["'_speed@float'"] = _speed;

	node["'_lerp_position@float'"] = _lerp_position;
}

void Hachiko::Scripting::DynamicCamera::OnLoad()
{
	if (load_node["'_start_point@math::float3'"].IsDefined())
	{
		_start_point = load_node["'_start_point@math::float3'"].as<math::float3>();
	}

	if (load_node["'_end_point@math::float3'"].IsDefined())
	{
		_end_point = load_node["'_end_point@math::float3'"].as<math::float3>();
	}

	if (load_node["'_speed@float'"].IsDefined())
	{
		_speed = load_node["'_speed@float'"].as<float>();
	}

	if (load_node["'_lerp_position@float'"].IsDefined())
	{
		_lerp_position = load_node["'_lerp_position@float'"].as<float>();
	}
}

void Hachiko::Scripting::EnemyController::OnSave(YAML::Node& node) const
{

	node["'_aggro_range@int'"] = _aggro_range;

	node["'_attack_range@int'"] = _attack_range;

	node["'_spawn_pos@float3'"] = _spawn_pos;

	node["'_spawn_is_initial@bool'"] = _spawn_is_initial;

	if (_player != nullptr)
	{
		node["'_player@GameObject*'"] = _player->GetID();
	}
	else
	{
		node["'_player@GameObject*'"] = 0;
	}

	node["'_attack_animation_duration@float'"] = _attack_animation_duration;

	node["'_attack_animation_timer@float'"] = _attack_animation_timer;
}

void Hachiko::Scripting::EnemyController::OnLoad()
{

	if (load_node["'_aggro_range@int'"].IsDefined())
	{
		_aggro_range = load_node["'_aggro_range@int'"].as<int>();
	}

	if (load_node["'_attack_range@int'"].IsDefined())
	{
		_attack_range = load_node["'_attack_range@int'"].as<int>();
	}

	if (load_node["'_spawn_pos@float3'"].IsDefined())
	{
		_spawn_pos = load_node["'_spawn_pos@float3'"].as<float3>();
	}

	if (load_node["'_spawn_is_initial@bool'"].IsDefined())
	{
		_spawn_is_initial = load_node["'_spawn_is_initial@bool'"].as<bool>();
	}

	if (load_node["'_player@GameObject*'"].IsDefined())
	{
		_player = SceneManagement::FindInCurrentScene(load_node["'_player@GameObject*'"].as<unsigned long long>());
	}

	if (load_node["'_attack_animation_duration@float'"].IsDefined())
	{
		_attack_animation_duration = load_node["'_attack_animation_duration@float'"].as<float>();
	}

	if (load_node["'_attack_animation_timer@float'"].IsDefined())
	{
		_attack_animation_timer = load_node["'_attack_animation_timer@float'"].as<float>();
	}
}

void Hachiko::Scripting::FancyLights::OnSave(YAML::Node& node) const
{
	node["'_rotate_on_y@bool'"] = _rotate_on_y;

	node["'_angle@float'"] = _angle;
}

void Hachiko::Scripting::FancyLights::OnLoad()
{
	if (load_node["'_rotate_on_y@bool'"].IsDefined())
	{
		_rotate_on_y = load_node["'_rotate_on_y@bool'"].as<bool>();
	}

	if (load_node["'_angle@float'"].IsDefined())
	{
		_angle = load_node["'_angle@float'"].as<float>();
	}
}

void Hachiko::Scripting::MainMenuManager::OnSave(YAML::Node& node) const
{

	node["'_state_changed@bool'"] = _state_changed;

	if (_main_background != nullptr)
	{
		node["'_main_background@GameObject*'"] = _main_background->GetID();
	}
	else
	{
		node["'_main_background@GameObject*'"] = 0;
	}

	if (_button_background != nullptr)
	{
		node["'_button_background@GameObject*'"] = _button_background->GetID();
	}
	else
	{
		node["'_button_background@GameObject*'"] = 0;
	}

	if (_button_play != nullptr && _button_play->GetGameObject() != nullptr)
	{
		node["'_button_play@ComponentButton*'"] = _button_play->GetGameObject()->GetID();
	}
	else
	{
		node["'_button_play@ComponentButton*'"] = 0;
	}

	if (_button_quit != nullptr && _button_quit->GetGameObject() != nullptr)
	{
		node["'_button_quit@ComponentButton*'"] = _button_quit->GetGameObject()->GetID();
	}
	else
	{
		node["'_button_quit@ComponentButton*'"] = 0;
	}

	if (_button_settings != nullptr && _button_settings->GetGameObject() != nullptr)
	{
		node["'_button_settings@ComponentButton*'"] = _button_settings->GetGameObject()->GetID();
	}
	else
	{
		node["'_button_settings@ComponentButton*'"] = 0;
	}

	if (_button_credits != nullptr && _button_credits->GetGameObject() != nullptr)
	{
		node["'_button_credits@ComponentButton*'"] = _button_credits->GetGameObject()->GetID();
	}
	else
	{
		node["'_button_credits@ComponentButton*'"] = 0;
	}

	if (_settings != nullptr)
	{
		node["'_settings@GameObject*'"] = _settings->GetID();
	}
	else
	{
		node["'_settings@GameObject*'"] = 0;
	}

	if (_credits != nullptr)
	{
		node["'_credits@GameObject*'"] = _credits->GetID();
	}
	else
	{
		node["'_credits@GameObject*'"] = 0;
	}

	if (_button_back != nullptr && _button_back->GetGameObject() != nullptr)
	{
		node["'_button_back@ComponentButton*'"] = _button_back->GetGameObject()->GetID();
	}
	else
	{
		node["'_button_back@ComponentButton*'"] = 0;
	}
}

void Hachiko::Scripting::MainMenuManager::OnLoad()
{

	if (load_node["'_state_changed@bool'"].IsDefined())
	{
		_state_changed = load_node["'_state_changed@bool'"].as<bool>();
	}

	if (load_node["'_main_background@GameObject*'"].IsDefined())
	{
		_main_background = SceneManagement::FindInCurrentScene(load_node["'_main_background@GameObject*'"].as<unsigned long long>());
	}

	if (load_node["'_button_background@GameObject*'"].IsDefined())
	{
		_button_background = SceneManagement::FindInCurrentScene(load_node["'_button_background@GameObject*'"].as<unsigned long long>());
	}

	if (load_node["'_button_play@ComponentButton*'"].IsDefined())
	{
		GameObject* _button_play_owner__temp = SceneManagement::FindInCurrentScene(load_node["'_button_play@ComponentButton*'"].as<unsigned long long>());
		if (_button_play_owner__temp != nullptr)
		{
			_button_play = _button_play_owner__temp->GetComponent<ComponentButton>();
		}
	}

	if (load_node["'_button_quit@ComponentButton*'"].IsDefined())
	{
		GameObject* _button_quit_owner__temp = SceneManagement::FindInCurrentScene(load_node["'_button_quit@ComponentButton*'"].as<unsigned long long>());
		if (_button_quit_owner__temp != nullptr)
		{
			_button_quit = _button_quit_owner__temp->GetComponent<ComponentButton>();
		}
	}

	if (load_node["'_button_settings@ComponentButton*'"].IsDefined())
	{
		GameObject* _button_settings_owner__temp = SceneManagement::FindInCurrentScene(load_node["'_button_settings@ComponentButton*'"].as<unsigned long long>());
		if (_button_settings_owner__temp != nullptr)
		{
			_button_settings = _button_settings_owner__temp->GetComponent<ComponentButton>();
		}
	}

	if (load_node["'_button_credits@ComponentButton*'"].IsDefined())
	{
		GameObject* _button_credits_owner__temp = SceneManagement::FindInCurrentScene(load_node["'_button_credits@ComponentButton*'"].as<unsigned long long>());
		if (_button_credits_owner__temp != nullptr)
		{
			_button_credits = _button_credits_owner__temp->GetComponent<ComponentButton>();
		}
	}

	if (load_node["'_settings@GameObject*'"].IsDefined())
	{
		_settings = SceneManagement::FindInCurrentScene(load_node["'_settings@GameObject*'"].as<unsigned long long>());
	}

	if (load_node["'_credits@GameObject*'"].IsDefined())
	{
		_credits = SceneManagement::FindInCurrentScene(load_node["'_credits@GameObject*'"].as<unsigned long long>());
	}

	if (load_node["'_button_back@ComponentButton*'"].IsDefined())
	{
		GameObject* _button_back_owner__temp = SceneManagement::FindInCurrentScene(load_node["'_button_back@ComponentButton*'"].as<unsigned long long>());
		if (_button_back_owner__temp != nullptr)
		{
			_button_back = _button_back_owner__temp->GetComponent<ComponentButton>();
		}
	}
}

void Hachiko::Scripting::PlayerAnimationManager::OnSave(YAML::Node& node) const
{
	if (_animator != nullptr && _animator->GetGameObject() != nullptr)
	{
		node["'_animator@ComponentAnimation*'"] = _animator->GetGameObject()->GetID();
	}
	else
	{
		node["'_animator@ComponentAnimation*'"] = 0;
	}

	node["'_state_string@std::string'"] = _state_string;

	node["'_idle_index@unsigned'"] = _idle_index;

	node["'_walking_index@unsigned'"] = _walking_index;

	node["'_dashing_index@unsigned'"] = _dashing_index;

	node["'_melee_index@unsigned'"] = _melee_index;

	node["'_ranged_index@unsigned'"] = _ranged_index;
}

void Hachiko::Scripting::PlayerAnimationManager::OnLoad()
{
	if (load_node["'_animator@ComponentAnimation*'"].IsDefined())
	{
		GameObject* _animator_owner__temp = SceneManagement::FindInCurrentScene(load_node["'_animator@ComponentAnimation*'"].as<unsigned long long>());
		if (_animator_owner__temp != nullptr)
		{
			_animator = _animator_owner__temp->GetComponent<ComponentAnimation>();
		}
	}

	if (load_node["'_state_string@std::string'"].IsDefined())
	{
		_state_string = load_node["'_state_string@std::string'"].as<std::string>();
	}

	if (load_node["'_idle_index@unsigned'"].IsDefined())
	{
		_idle_index = load_node["'_idle_index@unsigned'"].as<unsigned>();
	}

	if (load_node["'_walking_index@unsigned'"].IsDefined())
	{
		_walking_index = load_node["'_walking_index@unsigned'"].as<unsigned>();
	}

	if (load_node["'_dashing_index@unsigned'"].IsDefined())
	{
		_dashing_index = load_node["'_dashing_index@unsigned'"].as<unsigned>();
	}

	if (load_node["'_melee_index@unsigned'"].IsDefined())
	{
		_melee_index = load_node["'_melee_index@unsigned'"].as<unsigned>();
	}

	if (load_node["'_ranged_index@unsigned'"].IsDefined())
	{
		_ranged_index = load_node["'_ranged_index@unsigned'"].as<unsigned>();
	}
}

void Hachiko::Scripting::PlayerCamera::OnSave(YAML::Node& node) const
{
	node["'_relative_position_to_player@math::float3'"] = _relative_position_to_player;

	if (_player != nullptr)
	{
		node["'_player@GameObject*'"] = _player->GetID();
	}
	else
	{
		node["'_player@GameObject*'"] = 0;
	}

	node["'_follow_delay@float'"] = _follow_delay;
}

void Hachiko::Scripting::PlayerCamera::OnLoad()
{
	if (load_node["'_relative_position_to_player@math::float3'"].IsDefined())
	{
		_relative_position_to_player = load_node["'_relative_position_to_player@math::float3'"].as<math::float3>();
	}

	if (load_node["'_player@GameObject*'"].IsDefined())
	{
		_player = SceneManagement::FindInCurrentScene(load_node["'_player@GameObject*'"].as<unsigned long long>());
	}

	if (load_node["'_follow_delay@float'"].IsDefined())
	{
		_follow_delay = load_node["'_follow_delay@float'"].as<float>();
	}
}

void Hachiko::Scripting::PlayerController::OnSave(YAML::Node& node) const
{

	node["'_movement_speed@float'"] = _movement_speed;

	if (_attack_indicator != nullptr)
	{
		node["'_attack_indicator@GameObject*'"] = _attack_indicator->GetID();
	}
	else
	{
		node["'_attack_indicator@GameObject*'"] = 0;
	}

	if (_goal != nullptr)
	{
		node["'_goal@GameObject*'"] = _goal->GetID();
	}
	else
	{
		node["'_goal@GameObject*'"] = 0;
	}

	node["'_dash_duration@float'"] = _dash_duration;

	node["'_dash_distance@float'"] = _dash_distance;

	node["'_dash_cooldown@float'"] = _dash_cooldown;

	node["'_max_dash_charges@int'"] = _max_dash_charges;

	node["'_raycast_min_range@float'"] = _raycast_min_range;

	node["'_raycast_max_range@float'"] = _raycast_max_range;

	node["'_attack_radius@float'"] = _attack_radius;

	node["'_attack_cooldown@float'"] = _attack_cooldown;

	node["'_attack_duration@float'"] = _attack_duration;

	node["'_rotation_duration@float'"] = _rotation_duration;

	if (_camera != nullptr)
	{
		node["'_camera@GameObject*'"] = _camera->GetID();
	}
	else
	{
		node["'_camera@GameObject*'"] = 0;
	}

	if (_ui_damage != nullptr)
	{
		node["'_ui_damage@GameObject*'"] = _ui_damage->GetID();
	}
	else
	{
		node["'_ui_damage@GameObject*'"] = 0;
	}
}

void Hachiko::Scripting::PlayerController::OnLoad()
{

	if (load_node["'_movement_speed@float'"].IsDefined())
	{
		_movement_speed = load_node["'_movement_speed@float'"].as<float>();
	}

	if (load_node["'_attack_indicator@GameObject*'"].IsDefined())
	{
		_attack_indicator = SceneManagement::FindInCurrentScene(load_node["'_attack_indicator@GameObject*'"].as<unsigned long long>());
	}

	if (load_node["'_goal@GameObject*'"].IsDefined())
	{
		_goal = SceneManagement::FindInCurrentScene(load_node["'_goal@GameObject*'"].as<unsigned long long>());
	}

	if (load_node["'_dash_duration@float'"].IsDefined())
	{
		_dash_duration = load_node["'_dash_duration@float'"].as<float>();
	}

	if (load_node["'_dash_distance@float'"].IsDefined())
	{
		_dash_distance = load_node["'_dash_distance@float'"].as<float>();
	}

	if (load_node["'_dash_cooldown@float'"].IsDefined())
	{
		_dash_cooldown = load_node["'_dash_cooldown@float'"].as<float>();
	}

	if (load_node["'_max_dash_charges@int'"].IsDefined())
	{
		_max_dash_charges = load_node["'_max_dash_charges@int'"].as<int>();
	}

	if (load_node["'_raycast_min_range@float'"].IsDefined())
	{
		_raycast_min_range = load_node["'_raycast_min_range@float'"].as<float>();
	}

	if (load_node["'_raycast_max_range@float'"].IsDefined())
	{
		_raycast_max_range = load_node["'_raycast_max_range@float'"].as<float>();
	}

	if (load_node["'_attack_radius@float'"].IsDefined())
	{
		_attack_radius = load_node["'_attack_radius@float'"].as<float>();
	}

	if (load_node["'_attack_cooldown@float'"].IsDefined())
	{
		_attack_cooldown = load_node["'_attack_cooldown@float'"].as<float>();
	}

	if (load_node["'_attack_duration@float'"].IsDefined())
	{
		_attack_duration = load_node["'_attack_duration@float'"].as<float>();
	}

	if (load_node["'_rotation_duration@float'"].IsDefined())
	{
		_rotation_duration = load_node["'_rotation_duration@float'"].as<float>();
	}

	if (load_node["'_camera@GameObject*'"].IsDefined())
	{
		_camera = SceneManagement::FindInCurrentScene(load_node["'_camera@GameObject*'"].as<unsigned long long>());
	}

	if (load_node["'_ui_damage@GameObject*'"].IsDefined())
	{
		_ui_damage = SceneManagement::FindInCurrentScene(load_node["'_ui_damage@GameObject*'"].as<unsigned long long>());
	}
}

void Hachiko::Scripting::PlayerSoundManager::OnSave(YAML::Node& node) const
{
	if (_audio_source != nullptr && _audio_source->GetGameObject() != nullptr)
	{
		node["'_audio_source@ComponentAudioSource*'"] = _audio_source->GetGameObject()->GetID();
	}
	else
	{
		node["'_audio_source@ComponentAudioSource*'"] = 0;
	}

	node["'_step_frequency@float'"] = _step_frequency;

	node["'_melee_frequency@float'"] = _melee_frequency;

	node["'_ranged_frequency@float'"] = _ranged_frequency;

	node["'_timer@float'"] = _timer;
}

void Hachiko::Scripting::PlayerSoundManager::OnLoad()
{
	if (load_node["'_audio_source@ComponentAudioSource*'"].IsDefined())
	{
		GameObject* _audio_source_owner__temp = SceneManagement::FindInCurrentScene(load_node["'_audio_source@ComponentAudioSource*'"].as<unsigned long long>());
		if (_audio_source_owner__temp != nullptr)
		{
			_audio_source = _audio_source_owner__temp->GetComponent<ComponentAudioSource>();
		}
	}

	if (load_node["'_step_frequency@float'"].IsDefined())
	{
		_step_frequency = load_node["'_step_frequency@float'"].as<float>();
	}

	if (load_node["'_melee_frequency@float'"].IsDefined())
	{
		_melee_frequency = load_node["'_melee_frequency@float'"].as<float>();
	}

	if (load_node["'_ranged_frequency@float'"].IsDefined())
	{
		_ranged_frequency = load_node["'_ranged_frequency@float'"].as<float>();
	}

	if (load_node["'_timer@float'"].IsDefined())
	{
		_timer = load_node["'_timer@float'"].as<float>();
	}
}
