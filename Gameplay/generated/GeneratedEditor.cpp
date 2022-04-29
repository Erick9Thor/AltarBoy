#include "scriptingUtil/gameplaypch.h"
#include "BackToMainMenu.h"
#include "DynamicCamera.h"
#include "MainMenuManager.h"
#include "PlayerCamera.h"
#include "PlayerController.h"


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

void Hachiko::Scripting::PlayerCamera::OnEditor()
{
	Editor::Show("_distance_to_player", _distance_to_player);
	Editor::Show("_player", _player);
}

void Hachiko::Scripting::PlayerController::OnEditor()
{
	Editor::Show("_movement_speed", _movement_speed);
	Editor::Show("_dash_duration", _dash_duration);
	Editor::Show("_dash_distance", _dash_distance);
	Editor::Show("_is_dashing", _is_dashing);
	Editor::Show("_dash_progress", _dash_progress);
	Editor::Show("_dash_start", _dash_start);
	Editor::Show("_rotation_speed", _rotation_speed);
	Editor::Show("_is_falling", _is_falling);
	Editor::Show("_original_y", _original_y);
	Editor::Show("_speed_y", _speed_y);
	Editor::Show("_starting_position", _starting_position);
	Editor::Show("_double_field_test", _double_field_test);
	Editor::Show("_int_field_test", _int_field_test);
	Editor::Show("_unsigned_int_field_test", _unsigned_int_field_test);
	Editor::Show("_float2_field_test", _float2_field_test);
	Editor::Show("_float4_field_test", _float4_field_test);
	Editor::Show("_string_field_test", _string_field_test);
	Editor::Show("_game_object_field_test", _game_object_field_test);
	Editor::Show<ComponentButton>("_component_button_test", "ComponentButton*", _component_button_test);
}