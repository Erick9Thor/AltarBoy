#include "scriptingUtil/gameplaypch.h"
#include "BackToMainMenu.h"
#include "BugAnimationManager.h"
#include "CrystalExplosion.h"
#include "DynamicCamera.h"
#include "EnemyController.h"
#include "MainMenuManager.h"
#include "PlayerAnimationManager.h"
#include "PlayerCamera.h"
#include "PlayerController.h"
#include "PlayerSoundManager.h"
#include "RoomTeleporter.h"


void Hachiko::Scripting::BackToMainMenu::OnEditor()
{
	Editor::Show<ComponentButton>("_button_back", "ComponentButton*", _button_back);
}

void Hachiko::Scripting::BugAnimationManager::OnEditor()
{
	Editor::Show<ComponentAnimation>("_animator", "ComponentAnimation*", _animator);
	Editor::Show("_state_string", _state_string);
	Editor::Show("_idle_index", _idle_index);
	Editor::Show("_attacking_index", _attacking_index);
}

void Hachiko::Scripting::CrystalExplosion::OnEditor()
{
	Editor::Show("_player", _player);
	Editor::Show("_explosion_crystal", _explosion_crystal);
	Editor::Show("_static_crystal", _static_crystal);
	Editor::Show("_crashing_index", _crashing_index);
	Editor::Show("_detecting_radius", _detecting_radius);
	Editor::Show("_explosion_radius", _explosion_radius);
	Editor::Show("_explosive_crystal", _explosive_crystal);
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
	Editor::Show("_attack_animation_duration", _attack_animation_duration);
	Editor::Show("_attack_animation_timer", _attack_animation_timer);
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
	Editor::Show("_attack_indicator", _attack_indicator);
	Editor::Show("_goal", _goal);
	Editor::Show("_dash_duration", _dash_duration);
	Editor::Show("_dash_distance", _dash_distance);
	Editor::Show("_dash_cooldown", _dash_cooldown);
	Editor::Show("_max_dash_count", _max_dash_count);
	Editor::Show("_raycast_min_range", _raycast_min_range);
	Editor::Show("_raycast_max_range", _raycast_max_range);
	Editor::Show("_attack_radius", _attack_radius);
	Editor::Show("_attack_cooldown", _attack_cooldown);
	Editor::Show("_attack_duration", _attack_duration);
	Editor::Show("_rotation_duration", _rotation_duration);
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

void Hachiko::Scripting::RoomTeleporter::OnEditor()
{
	Editor::Show("_touching", _touching);
	Editor::Show("_target", _target);
	Editor::Show<ComponentImage>("_fade_image", "ComponentImage*", _fade_image);
}