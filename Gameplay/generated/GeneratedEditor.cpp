#include "scriptingUtil/gameplaypch.h"
#include "BackToMainMenu.h"
#include "DynamicCamera.h"
#include "EnemyController.h"
#include "MainMenuManager.h"
#include "PlayerAnimationManager.h"
#include "PlayerCamera.h"
#include "PlayerController.h"
#include "PlayerSoundManager.h"


void Hachiko::Scripting::BackToMainMenu::OnEditor()
{
	Editor::Show<ComponentButton>("_button_back", "ComponentButton*", _button_back);
}

void Hachiko::Scripting::DynamicCamera::OnEditor()
{
	Editor::Show("_start_point", _start_point);
	Editor::Show("_end_point", _end_point);
	Editor::Show("_speed", _speed);
	Editor::Show("_lerp_position", _lerp_position);
}

void Hachiko::Scripting::EnemyController::OnEditor()
{
	Editor::Show("_aggro_range", _aggro_range);
	Editor::Show("_attack_range", _attack_range);
	Editor::Show("_spawn_pos", _spawn_pos);
	Editor::Show("_spawn_is_initial", _spawn_is_initial);
	Editor::Show("_player", _player);
}

void Hachiko::Scripting::MainMenuManager::OnEditor()
{
	Editor::Show("_state_changed", _state_changed);
	Editor::Show("_main_background", _main_background);
	Editor::Show("_button_background", _button_background);
	Editor::Show<ComponentButton>("_button_play", "ComponentButton*", _button_play);
	Editor::Show<ComponentButton>("_button_quit", "ComponentButton*", _button_quit);
	Editor::Show<ComponentButton>("_button_settings", "ComponentButton*", _button_settings);
	Editor::Show<ComponentButton>("_button_credits", "ComponentButton*", _button_credits);
	Editor::Show("_settings", _settings);
	Editor::Show("_credits", _credits);
	Editor::Show<ComponentButton>("_button_back", "ComponentButton*", _button_back);
}

void Hachiko::Scripting::PlayerAnimationManager::OnEditor()
{
	Editor::Show<ComponentAnimation>("_animator", "ComponentAnimation*", _animator);
	Editor::Show("_state_string", _state_string);
	Editor::Show("_idle_index", _idle_index);
	Editor::Show("_walking_index", _walking_index);
	Editor::Show("_dashing_index", _dashing_index);
	Editor::Show("_melee_index", _melee_index);
	Editor::Show("_ranged_index", _ranged_index);
}

void Hachiko::Scripting::PlayerCamera::OnEditor()
{
	Editor::Show("_relative_position_to_player", _relative_position_to_player);
	Editor::Show("_player", _player);
	Editor::Show("_follow_delay", _follow_delay);
}

void Hachiko::Scripting::PlayerController::OnEditor()
{
	Editor::Show("_movement_speed", _movement_speed);
	Editor::Show("_dash_indicator", _dash_indicator);
	Editor::Show("_goal", _goal);
	Editor::Show("_dash_duration", _dash_duration);
	Editor::Show("_dash_distance", _dash_distance);
	Editor::Show("_dash_progress", _dash_progress);
	Editor::Show("_dash_cooldown", _dash_cooldown);
	Editor::Show("_dash_timer", _dash_timer);
	Editor::Show("_dash_count", _dash_count);
	Editor::Show("_max_dash_count", _max_dash_count);
	Editor::Show("_is_dashing", _is_dashing);
	Editor::Show("_has_cooldown", _has_cooldown);
	Editor::Show("_is_falling", _is_falling);
	Editor::Show("_dash_start", _dash_start);
	Editor::Show("_dash_direction", _dash_direction);
	Editor::Show("_raycast_min_range", _raycast_min_range);
	Editor::Show("_raycast_max_range", _raycast_max_range);
	Editor::Show("_attack_radius", _attack_radius);
	Editor::Show("_attack_cooldown", _attack_cooldown);
	Editor::Show("_should_rotate", _should_rotate);
	Editor::Show("_rotation_progress", _rotation_progress);
	Editor::Show("_rotation_duration", _rotation_duration);
	Editor::Show("_rotation_start", _rotation_start);
	Editor::Show("_rotation_target", _rotation_target);
	Editor::Show("_camera", _camera);
	Editor::Show("_ui_damage", _ui_damage);
}

void Hachiko::Scripting::PlayerSoundManager::OnEditor()
{
	Editor::Show<ComponentAudioSource>("_audio_source", "ComponentAudioSource*", _audio_source);
	Editor::Show("_step_frequency", _step_frequency);
	Editor::Show("_melee_frequency", _melee_frequency);
	Editor::Show("_ranged_frequency", _ranged_frequency);
	Editor::Show("_timer", _timer);
}