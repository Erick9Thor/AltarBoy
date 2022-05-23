#include <vector>
#include <string>
#include <unordered_map>
#include "scriptingUtil/gameplaypch.h"
#include "BackToMainMenu.h"
#include "DynamicCamera.h"
#include "EnemyController.h"
#include "MainMenuManager.h"
#include "PlayerAnimationManager.h"
#include "PlayerCamera.h"
#include "PlayerController.h"
#include "PlayerSoundManager.h"


void Hachiko::Scripting::BackToMainMenu::DeserializeFrom(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::DeserializeFrom(serialized_fields);

	if(serialized_fields.find("_button_back") != serialized_fields.end())
	{
		const SerializedField& _button_back_sf = serialized_fields["_button_back"];
		if (_button_back_sf.type_name == "ComponentButton*")
		{
			_button_back = std::any_cast<ComponentButton*>(_button_back_sf.copy);
		}
	}
}

void Hachiko::Scripting::BackToMainMenu::SerializeTo(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::SerializeTo(serialized_fields);

	serialized_fields["_button_back"] = SerializedField(std::string("_button_back"), std::make_any<ComponentButton*>(_button_back), std::string("ComponentButton*"));
}

void Hachiko::Scripting::DynamicCamera::DeserializeFrom(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::DeserializeFrom(serialized_fields);

	if(serialized_fields.find("_start_point") != serialized_fields.end())
	{
		const SerializedField& _start_point_sf = serialized_fields["_start_point"];
		if (_start_point_sf.type_name == "math::float3")
		{
			_start_point = std::any_cast<math::float3>(_start_point_sf.copy);
		}
	}

	if(serialized_fields.find("_end_point") != serialized_fields.end())
	{
		const SerializedField& _end_point_sf = serialized_fields["_end_point"];
		if (_end_point_sf.type_name == "math::float3")
		{
			_end_point = std::any_cast<math::float3>(_end_point_sf.copy);
		}
	}

	if(serialized_fields.find("_speed") != serialized_fields.end())
	{
		const SerializedField& _speed_sf = serialized_fields["_speed"];
		if (_speed_sf.type_name == "float")
		{
			_speed = std::any_cast<float>(_speed_sf.copy);
		}
	}

	if(serialized_fields.find("_lerp_position") != serialized_fields.end())
	{
		const SerializedField& _lerp_position_sf = serialized_fields["_lerp_position"];
		if (_lerp_position_sf.type_name == "float")
		{
			_lerp_position = std::any_cast<float>(_lerp_position_sf.copy);
		}
	}
}

void Hachiko::Scripting::DynamicCamera::SerializeTo(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::SerializeTo(serialized_fields);

	serialized_fields["_start_point"] = SerializedField(std::string("_start_point"), std::make_any<math::float3>(_start_point), std::string("math::float3"));

	serialized_fields["_end_point"] = SerializedField(std::string("_end_point"), std::make_any<math::float3>(_end_point), std::string("math::float3"));

	serialized_fields["_speed"] = SerializedField(std::string("_speed"), std::make_any<float>(_speed), std::string("float"));

	serialized_fields["_lerp_position"] = SerializedField(std::string("_lerp_position"), std::make_any<float>(_lerp_position), std::string("float"));
}

void Hachiko::Scripting::EnemyController::DeserializeFrom(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::DeserializeFrom(serialized_fields);

	if(serialized_fields.find("_stats") != serialized_fields.end())
	{
		const SerializedField& _stats_sf = serialized_fields["_stats"];
		if (_stats_sf.type_name == "Stats")
		{
			_stats = std::any_cast<Stats>(_stats_sf.copy);
		}
	}

	if(serialized_fields.find("_aggro_range") != serialized_fields.end())
	{
		const SerializedField& _aggro_range_sf = serialized_fields["_aggro_range"];
		if (_aggro_range_sf.type_name == "int")
		{
			_aggro_range = std::any_cast<int>(_aggro_range_sf.copy);
		}
	}

	if(serialized_fields.find("_attack_range") != serialized_fields.end())
	{
		const SerializedField& _attack_range_sf = serialized_fields["_attack_range"];
		if (_attack_range_sf.type_name == "int")
		{
			_attack_range = std::any_cast<int>(_attack_range_sf.copy);
		}
	}

	if(serialized_fields.find("_spawn_pos") != serialized_fields.end())
	{
		const SerializedField& _spawn_pos_sf = serialized_fields["_spawn_pos"];
		if (_spawn_pos_sf.type_name == "float3")
		{
			_spawn_pos = std::any_cast<float3>(_spawn_pos_sf.copy);
		}
	}
}

void Hachiko::Scripting::EnemyController::SerializeTo(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::SerializeTo(serialized_fields);

	serialized_fields["_stats"] = SerializedField(std::string("_stats"), std::make_any<Stats>(_stats), std::string("Stats"));

	serialized_fields["_aggro_range"] = SerializedField(std::string("_aggro_range"), std::make_any<int>(_aggro_range), std::string("int"));

	serialized_fields["_attack_range"] = SerializedField(std::string("_attack_range"), std::make_any<int>(_attack_range), std::string("int"));

	serialized_fields["_spawn_pos"] = SerializedField(std::string("_spawn_pos"), std::make_any<float3>(_spawn_pos), std::string("float3"));
}

void Hachiko::Scripting::MainMenuManager::DeserializeFrom(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::DeserializeFrom(serialized_fields);

	if(serialized_fields.find("_state") != serialized_fields.end())
	{
		const SerializedField& _state_sf = serialized_fields["_state"];
		if (_state_sf.type_name == "State")
		{
			_state = std::any_cast<State>(_state_sf.copy);
		}
	}

	if(serialized_fields.find("_state_changed") != serialized_fields.end())
	{
		const SerializedField& _state_changed_sf = serialized_fields["_state_changed"];
		if (_state_changed_sf.type_name == "bool")
		{
			_state_changed = std::any_cast<bool>(_state_changed_sf.copy);
		}
	}

	if(serialized_fields.find("_main_background") != serialized_fields.end())
	{
		const SerializedField& _main_background_sf = serialized_fields["_main_background"];
		if (_main_background_sf.type_name == "GameObject*")
		{
			_main_background = std::any_cast<GameObject*>(_main_background_sf.copy);
		}
	}

	if(serialized_fields.find("_button_background") != serialized_fields.end())
	{
		const SerializedField& _button_background_sf = serialized_fields["_button_background"];
		if (_button_background_sf.type_name == "GameObject*")
		{
			_button_background = std::any_cast<GameObject*>(_button_background_sf.copy);
		}
	}

	if(serialized_fields.find("_button_play") != serialized_fields.end())
	{
		const SerializedField& _button_play_sf = serialized_fields["_button_play"];
		if (_button_play_sf.type_name == "ComponentButton*")
		{
			_button_play = std::any_cast<ComponentButton*>(_button_play_sf.copy);
		}
	}

	if(serialized_fields.find("_button_quit") != serialized_fields.end())
	{
		const SerializedField& _button_quit_sf = serialized_fields["_button_quit"];
		if (_button_quit_sf.type_name == "ComponentButton*")
		{
			_button_quit = std::any_cast<ComponentButton*>(_button_quit_sf.copy);
		}
	}

	if(serialized_fields.find("_button_settings") != serialized_fields.end())
	{
		const SerializedField& _button_settings_sf = serialized_fields["_button_settings"];
		if (_button_settings_sf.type_name == "ComponentButton*")
		{
			_button_settings = std::any_cast<ComponentButton*>(_button_settings_sf.copy);
		}
	}

	if(serialized_fields.find("_button_credits") != serialized_fields.end())
	{
		const SerializedField& _button_credits_sf = serialized_fields["_button_credits"];
		if (_button_credits_sf.type_name == "ComponentButton*")
		{
			_button_credits = std::any_cast<ComponentButton*>(_button_credits_sf.copy);
		}
	}

	if(serialized_fields.find("_settings") != serialized_fields.end())
	{
		const SerializedField& _settings_sf = serialized_fields["_settings"];
		if (_settings_sf.type_name == "GameObject*")
		{
			_settings = std::any_cast<GameObject*>(_settings_sf.copy);
		}
	}

	if(serialized_fields.find("_credits") != serialized_fields.end())
	{
		const SerializedField& _credits_sf = serialized_fields["_credits"];
		if (_credits_sf.type_name == "GameObject*")
		{
			_credits = std::any_cast<GameObject*>(_credits_sf.copy);
		}
	}

	if(serialized_fields.find("_button_back") != serialized_fields.end())
	{
		const SerializedField& _button_back_sf = serialized_fields["_button_back"];
		if (_button_back_sf.type_name == "ComponentButton*")
		{
			_button_back = std::any_cast<ComponentButton*>(_button_back_sf.copy);
		}
	}
}

void Hachiko::Scripting::MainMenuManager::SerializeTo(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::SerializeTo(serialized_fields);

	serialized_fields["_state"] = SerializedField(std::string("_state"), std::make_any<State>(_state), std::string("State"));

	serialized_fields["_state_changed"] = SerializedField(std::string("_state_changed"), std::make_any<bool>(_state_changed), std::string("bool"));

	serialized_fields["_main_background"] = SerializedField(std::string("_main_background"), std::make_any<GameObject*>(_main_background), std::string("GameObject*"));

	serialized_fields["_button_background"] = SerializedField(std::string("_button_background"), std::make_any<GameObject*>(_button_background), std::string("GameObject*"));

	serialized_fields["_button_play"] = SerializedField(std::string("_button_play"), std::make_any<ComponentButton*>(_button_play), std::string("ComponentButton*"));

	serialized_fields["_button_quit"] = SerializedField(std::string("_button_quit"), std::make_any<ComponentButton*>(_button_quit), std::string("ComponentButton*"));

	serialized_fields["_button_settings"] = SerializedField(std::string("_button_settings"), std::make_any<ComponentButton*>(_button_settings), std::string("ComponentButton*"));

	serialized_fields["_button_credits"] = SerializedField(std::string("_button_credits"), std::make_any<ComponentButton*>(_button_credits), std::string("ComponentButton*"));

	serialized_fields["_settings"] = SerializedField(std::string("_settings"), std::make_any<GameObject*>(_settings), std::string("GameObject*"));

	serialized_fields["_credits"] = SerializedField(std::string("_credits"), std::make_any<GameObject*>(_credits), std::string("GameObject*"));

	serialized_fields["_button_back"] = SerializedField(std::string("_button_back"), std::make_any<ComponentButton*>(_button_back), std::string("ComponentButton*"));
}

void Hachiko::Scripting::PlayerAnimationManager::DeserializeFrom(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::DeserializeFrom(serialized_fields);

	if(serialized_fields.find("_animator") != serialized_fields.end())
	{
		const SerializedField& _animator_sf = serialized_fields["_animator"];
		if (_animator_sf.type_name == "ComponentAnimation*")
		{
			_animator = std::any_cast<ComponentAnimation*>(_animator_sf.copy);
		}
	}

	if(serialized_fields.find("_previous_state") != serialized_fields.end())
	{
		const SerializedField& _previous_state_sf = serialized_fields["_previous_state"];
		if (_previous_state_sf.type_name == "PlayerState")
		{
			_previous_state = std::any_cast<PlayerState>(_previous_state_sf.copy);
		}
	}

	if(serialized_fields.find("_state_string") != serialized_fields.end())
	{
		const SerializedField& _state_string_sf = serialized_fields["_state_string"];
		if (_state_string_sf.type_name == "std::string")
		{
			_state_string = std::any_cast<std::string>(_state_string_sf.copy);
		}
	}

	if(serialized_fields.find("_idle_index") != serialized_fields.end())
	{
		const SerializedField& _idle_index_sf = serialized_fields["_idle_index"];
		if (_idle_index_sf.type_name == "unsigned")
		{
			_idle_index = std::any_cast<unsigned>(_idle_index_sf.copy);
		}
	}

	if(serialized_fields.find("_walking_index") != serialized_fields.end())
	{
		const SerializedField& _walking_index_sf = serialized_fields["_walking_index"];
		if (_walking_index_sf.type_name == "unsigned")
		{
			_walking_index = std::any_cast<unsigned>(_walking_index_sf.copy);
		}
	}

	if(serialized_fields.find("_dashing_index") != serialized_fields.end())
	{
		const SerializedField& _dashing_index_sf = serialized_fields["_dashing_index"];
		if (_dashing_index_sf.type_name == "unsigned")
		{
			_dashing_index = std::any_cast<unsigned>(_dashing_index_sf.copy);
		}
	}

	if(serialized_fields.find("_melee_index") != serialized_fields.end())
	{
		const SerializedField& _melee_index_sf = serialized_fields["_melee_index"];
		if (_melee_index_sf.type_name == "unsigned")
		{
			_melee_index = std::any_cast<unsigned>(_melee_index_sf.copy);
		}
	}

	if(serialized_fields.find("_ranged_index") != serialized_fields.end())
	{
		const SerializedField& _ranged_index_sf = serialized_fields["_ranged_index"];
		if (_ranged_index_sf.type_name == "unsigned")
		{
			_ranged_index = std::any_cast<unsigned>(_ranged_index_sf.copy);
		}
	}
}

void Hachiko::Scripting::PlayerAnimationManager::SerializeTo(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::SerializeTo(serialized_fields);

	serialized_fields["_animator"] = SerializedField(std::string("_animator"), std::make_any<ComponentAnimation*>(_animator), std::string("ComponentAnimation*"));

	serialized_fields["_previous_state"] = SerializedField(std::string("_previous_state"), std::make_any<PlayerState>(_previous_state), std::string("PlayerState"));

	serialized_fields["_state_string"] = SerializedField(std::string("_state_string"), std::make_any<std::string>(_state_string), std::string("std::string"));

	serialized_fields["_idle_index"] = SerializedField(std::string("_idle_index"), std::make_any<unsigned>(_idle_index), std::string("unsigned"));

	serialized_fields["_walking_index"] = SerializedField(std::string("_walking_index"), std::make_any<unsigned>(_walking_index), std::string("unsigned"));

	serialized_fields["_dashing_index"] = SerializedField(std::string("_dashing_index"), std::make_any<unsigned>(_dashing_index), std::string("unsigned"));

	serialized_fields["_melee_index"] = SerializedField(std::string("_melee_index"), std::make_any<unsigned>(_melee_index), std::string("unsigned"));

	serialized_fields["_ranged_index"] = SerializedField(std::string("_ranged_index"), std::make_any<unsigned>(_ranged_index), std::string("unsigned"));
}

void Hachiko::Scripting::PlayerCamera::DeserializeFrom(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::DeserializeFrom(serialized_fields);

	if(serialized_fields.find("_relative_position_to_player") != serialized_fields.end())
	{
		const SerializedField& _relative_position_to_player_sf = serialized_fields["_relative_position_to_player"];
		if (_relative_position_to_player_sf.type_name == "math::float3")
		{
			_relative_position_to_player = std::any_cast<math::float3>(_relative_position_to_player_sf.copy);
		}
	}

	if(serialized_fields.find("_player") != serialized_fields.end())
	{
		const SerializedField& _player_sf = serialized_fields["_player"];
		if (_player_sf.type_name == "GameObject*")
		{
			_player = std::any_cast<GameObject*>(_player_sf.copy);
		}
	}

	if(serialized_fields.find("_follow_delay") != serialized_fields.end())
	{
		const SerializedField& _follow_delay_sf = serialized_fields["_follow_delay"];
		if (_follow_delay_sf.type_name == "float")
		{
			_follow_delay = std::any_cast<float>(_follow_delay_sf.copy);
		}
	}
}

void Hachiko::Scripting::PlayerCamera::SerializeTo(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::SerializeTo(serialized_fields);

	serialized_fields["_relative_position_to_player"] = SerializedField(std::string("_relative_position_to_player"), std::make_any<math::float3>(_relative_position_to_player), std::string("math::float3"));

	serialized_fields["_player"] = SerializedField(std::string("_player"), std::make_any<GameObject*>(_player), std::string("GameObject*"));

	serialized_fields["_follow_delay"] = SerializedField(std::string("_follow_delay"), std::make_any<float>(_follow_delay), std::string("float"));
}

void Hachiko::Scripting::PlayerController::DeserializeFrom(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::DeserializeFrom(serialized_fields);

	if(serialized_fields.find("_stats") != serialized_fields.end())
	{
		const SerializedField& _stats_sf = serialized_fields["_stats"];
		if (_stats_sf.type_name == "Stats")
		{
			_stats = std::any_cast<Stats>(_stats_sf.copy);
		}
	}

	if(serialized_fields.find("_movement_speed") != serialized_fields.end())
	{
		const SerializedField& _movement_speed_sf = serialized_fields["_movement_speed"];
		if (_movement_speed_sf.type_name == "float")
		{
			_movement_speed = std::any_cast<float>(_movement_speed_sf.copy);
		}
	}

	if(serialized_fields.find("_dash_indicator") != serialized_fields.end())
	{
		const SerializedField& _dash_indicator_sf = serialized_fields["_dash_indicator"];
		if (_dash_indicator_sf.type_name == "GameObject*")
		{
			_dash_indicator = std::any_cast<GameObject*>(_dash_indicator_sf.copy);
		}
	}

	if(serialized_fields.find("_goal") != serialized_fields.end())
	{
		const SerializedField& _goal_sf = serialized_fields["_goal"];
		if (_goal_sf.type_name == "GameObject*")
		{
			_goal = std::any_cast<GameObject*>(_goal_sf.copy);
		}
	}

	if(serialized_fields.find("_dash_duration") != serialized_fields.end())
	{
		const SerializedField& _dash_duration_sf = serialized_fields["_dash_duration"];
		if (_dash_duration_sf.type_name == "float")
		{
			_dash_duration = std::any_cast<float>(_dash_duration_sf.copy);
		}
	}

	if(serialized_fields.find("_dash_distance") != serialized_fields.end())
	{
		const SerializedField& _dash_distance_sf = serialized_fields["_dash_distance"];
		if (_dash_distance_sf.type_name == "float")
		{
			_dash_distance = std::any_cast<float>(_dash_distance_sf.copy);
		}
	}

	if(serialized_fields.find("_dash_progress") != serialized_fields.end())
	{
		const SerializedField& _dash_progress_sf = serialized_fields["_dash_progress"];
		if (_dash_progress_sf.type_name == "float")
		{
			_dash_progress = std::any_cast<float>(_dash_progress_sf.copy);
		}
	}

	if(serialized_fields.find("_dash_cooldown") != serialized_fields.end())
	{
		const SerializedField& _dash_cooldown_sf = serialized_fields["_dash_cooldown"];
		if (_dash_cooldown_sf.type_name == "float")
		{
			_dash_cooldown = std::any_cast<float>(_dash_cooldown_sf.copy);
		}
	}

	if(serialized_fields.find("_dash_timer") != serialized_fields.end())
	{
		const SerializedField& _dash_timer_sf = serialized_fields["_dash_timer"];
		if (_dash_timer_sf.type_name == "float")
		{
			_dash_timer = std::any_cast<float>(_dash_timer_sf.copy);
		}
	}

	if(serialized_fields.find("_dash_count") != serialized_fields.end())
	{
		const SerializedField& _dash_count_sf = serialized_fields["_dash_count"];
		if (_dash_count_sf.type_name == "int")
		{
			_dash_count = std::any_cast<int>(_dash_count_sf.copy);
		}
	}

	if(serialized_fields.find("_max_dash_count") != serialized_fields.end())
	{
		const SerializedField& _max_dash_count_sf = serialized_fields["_max_dash_count"];
		if (_max_dash_count_sf.type_name == "int")
		{
			_max_dash_count = std::any_cast<int>(_max_dash_count_sf.copy);
		}
	}

	if(serialized_fields.find("_is_dashing") != serialized_fields.end())
	{
		const SerializedField& _is_dashing_sf = serialized_fields["_is_dashing"];
		if (_is_dashing_sf.type_name == "bool")
		{
			_is_dashing = std::any_cast<bool>(_is_dashing_sf.copy);
		}
	}

	if(serialized_fields.find("_has_cooldown") != serialized_fields.end())
	{
		const SerializedField& _has_cooldown_sf = serialized_fields["_has_cooldown"];
		if (_has_cooldown_sf.type_name == "bool")
		{
			_has_cooldown = std::any_cast<bool>(_has_cooldown_sf.copy);
		}
	}

	if(serialized_fields.find("_is_falling") != serialized_fields.end())
	{
		const SerializedField& _is_falling_sf = serialized_fields["_is_falling"];
		if (_is_falling_sf.type_name == "bool")
		{
			_is_falling = std::any_cast<bool>(_is_falling_sf.copy);
		}
	}

	if(serialized_fields.find("_dash_start") != serialized_fields.end())
	{
		const SerializedField& _dash_start_sf = serialized_fields["_dash_start"];
		if (_dash_start_sf.type_name == "math::float3")
		{
			_dash_start = std::any_cast<math::float3>(_dash_start_sf.copy);
		}
	}

	if(serialized_fields.find("_dash_direction") != serialized_fields.end())
	{
		const SerializedField& _dash_direction_sf = serialized_fields["_dash_direction"];
		if (_dash_direction_sf.type_name == "math::float3")
		{
			_dash_direction = std::any_cast<math::float3>(_dash_direction_sf.copy);
		}
	}

	if(serialized_fields.find("_raycast_min_range") != serialized_fields.end())
	{
		const SerializedField& _raycast_min_range_sf = serialized_fields["_raycast_min_range"];
		if (_raycast_min_range_sf.type_name == "float")
		{
			_raycast_min_range = std::any_cast<float>(_raycast_min_range_sf.copy);
		}
	}

	if(serialized_fields.find("_raycast_max_range") != serialized_fields.end())
	{
		const SerializedField& _raycast_max_range_sf = serialized_fields["_raycast_max_range"];
		if (_raycast_max_range_sf.type_name == "float")
		{
			_raycast_max_range = std::any_cast<float>(_raycast_max_range_sf.copy);
		}
	}

	if(serialized_fields.find("_attack_radius") != serialized_fields.end())
	{
		const SerializedField& _attack_radius_sf = serialized_fields["_attack_radius"];
		if (_attack_radius_sf.type_name == "float")
		{
			_attack_radius = std::any_cast<float>(_attack_radius_sf.copy);
		}
	}

	if(serialized_fields.find("_attack_cooldown") != serialized_fields.end())
	{
		const SerializedField& _attack_cooldown_sf = serialized_fields["_attack_cooldown"];
		if (_attack_cooldown_sf.type_name == "float")
		{
			_attack_cooldown = std::any_cast<float>(_attack_cooldown_sf.copy);
		}
	}

	if(serialized_fields.find("_should_rotate") != serialized_fields.end())
	{
		const SerializedField& _should_rotate_sf = serialized_fields["_should_rotate"];
		if (_should_rotate_sf.type_name == "bool")
		{
			_should_rotate = std::any_cast<bool>(_should_rotate_sf.copy);
		}
	}

	if(serialized_fields.find("_rotation_progress") != serialized_fields.end())
	{
		const SerializedField& _rotation_progress_sf = serialized_fields["_rotation_progress"];
		if (_rotation_progress_sf.type_name == "float")
		{
			_rotation_progress = std::any_cast<float>(_rotation_progress_sf.copy);
		}
	}

	if(serialized_fields.find("_rotation_duration") != serialized_fields.end())
	{
		const SerializedField& _rotation_duration_sf = serialized_fields["_rotation_duration"];
		if (_rotation_duration_sf.type_name == "float")
		{
			_rotation_duration = std::any_cast<float>(_rotation_duration_sf.copy);
		}
	}

	if(serialized_fields.find("_rotation_start") != serialized_fields.end())
	{
		const SerializedField& _rotation_start_sf = serialized_fields["_rotation_start"];
		if (_rotation_start_sf.type_name == "math::Quat")
		{
			_rotation_start = std::any_cast<math::Quat>(_rotation_start_sf.copy);
		}
	}

	if(serialized_fields.find("_rotation_target") != serialized_fields.end())
	{
		const SerializedField& _rotation_target_sf = serialized_fields["_rotation_target"];
		if (_rotation_target_sf.type_name == "math::Quat")
		{
			_rotation_target = std::any_cast<math::Quat>(_rotation_target_sf.copy);
		}
	}

	if(serialized_fields.find("_state") != serialized_fields.end())
	{
		const SerializedField& _state_sf = serialized_fields["_state"];
		if (_state_sf.type_name == "PlayerState")
		{
			_state = std::any_cast<PlayerState>(_state_sf.copy);
		}
	}
}

void Hachiko::Scripting::PlayerController::SerializeTo(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::SerializeTo(serialized_fields);

	serialized_fields["_stats"] = SerializedField(std::string("_stats"), std::make_any<Stats>(_stats), std::string("Stats"));

	serialized_fields["_movement_speed"] = SerializedField(std::string("_movement_speed"), std::make_any<float>(_movement_speed), std::string("float"));

	serialized_fields["_dash_indicator"] = SerializedField(std::string("_dash_indicator"), std::make_any<GameObject*>(_dash_indicator), std::string("GameObject*"));

	serialized_fields["_goal"] = SerializedField(std::string("_goal"), std::make_any<GameObject*>(_goal), std::string("GameObject*"));

	serialized_fields["_dash_duration"] = SerializedField(std::string("_dash_duration"), std::make_any<float>(_dash_duration), std::string("float"));

	serialized_fields["_dash_distance"] = SerializedField(std::string("_dash_distance"), std::make_any<float>(_dash_distance), std::string("float"));

	serialized_fields["_dash_progress"] = SerializedField(std::string("_dash_progress"), std::make_any<float>(_dash_progress), std::string("float"));

	serialized_fields["_dash_cooldown"] = SerializedField(std::string("_dash_cooldown"), std::make_any<float>(_dash_cooldown), std::string("float"));

	serialized_fields["_dash_timer"] = SerializedField(std::string("_dash_timer"), std::make_any<float>(_dash_timer), std::string("float"));

	serialized_fields["_dash_count"] = SerializedField(std::string("_dash_count"), std::make_any<int>(_dash_count), std::string("int"));

	serialized_fields["_max_dash_count"] = SerializedField(std::string("_max_dash_count"), std::make_any<int>(_max_dash_count), std::string("int"));

	serialized_fields["_is_dashing"] = SerializedField(std::string("_is_dashing"), std::make_any<bool>(_is_dashing), std::string("bool"));

	serialized_fields["_has_cooldown"] = SerializedField(std::string("_has_cooldown"), std::make_any<bool>(_has_cooldown), std::string("bool"));

	serialized_fields["_is_falling"] = SerializedField(std::string("_is_falling"), std::make_any<bool>(_is_falling), std::string("bool"));

	serialized_fields["_dash_start"] = SerializedField(std::string("_dash_start"), std::make_any<math::float3>(_dash_start), std::string("math::float3"));

	serialized_fields["_dash_direction"] = SerializedField(std::string("_dash_direction"), std::make_any<math::float3>(_dash_direction), std::string("math::float3"));

	serialized_fields["_raycast_min_range"] = SerializedField(std::string("_raycast_min_range"), std::make_any<float>(_raycast_min_range), std::string("float"));

	serialized_fields["_raycast_max_range"] = SerializedField(std::string("_raycast_max_range"), std::make_any<float>(_raycast_max_range), std::string("float"));

	serialized_fields["_attack_radius"] = SerializedField(std::string("_attack_radius"), std::make_any<float>(_attack_radius), std::string("float"));

	serialized_fields["_attack_cooldown"] = SerializedField(std::string("_attack_cooldown"), std::make_any<float>(_attack_cooldown), std::string("float"));

	serialized_fields["_should_rotate"] = SerializedField(std::string("_should_rotate"), std::make_any<bool>(_should_rotate), std::string("bool"));

	serialized_fields["_rotation_progress"] = SerializedField(std::string("_rotation_progress"), std::make_any<float>(_rotation_progress), std::string("float"));

	serialized_fields["_rotation_duration"] = SerializedField(std::string("_rotation_duration"), std::make_any<float>(_rotation_duration), std::string("float"));

	serialized_fields["_rotation_start"] = SerializedField(std::string("_rotation_start"), std::make_any<math::Quat>(_rotation_start), std::string("math::Quat"));

	serialized_fields["_rotation_target"] = SerializedField(std::string("_rotation_target"), std::make_any<math::Quat>(_rotation_target), std::string("math::Quat"));

	serialized_fields["_state"] = SerializedField(std::string("_state"), std::make_any<PlayerState>(_state), std::string("PlayerState"));
}

void Hachiko::Scripting::PlayerSoundManager::DeserializeFrom(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::DeserializeFrom(serialized_fields);

	if(serialized_fields.find("_step_frequency") != serialized_fields.end())
	{
		const SerializedField& _step_frequency_sf = serialized_fields["_step_frequency"];
		if (_step_frequency_sf.type_name == "float")
		{
			_step_frequency = std::any_cast<float>(_step_frequency_sf.copy);
		}
	}

	if(serialized_fields.find("_melee_frequency") != serialized_fields.end())
	{
		const SerializedField& _melee_frequency_sf = serialized_fields["_melee_frequency"];
		if (_melee_frequency_sf.type_name == "float")
		{
			_melee_frequency = std::any_cast<float>(_melee_frequency_sf.copy);
		}
	}

	if(serialized_fields.find("_ranged_frequency") != serialized_fields.end())
	{
		const SerializedField& _ranged_frequency_sf = serialized_fields["_ranged_frequency"];
		if (_ranged_frequency_sf.type_name == "float")
		{
			_ranged_frequency = std::any_cast<float>(_ranged_frequency_sf.copy);
		}
	}

	if(serialized_fields.find("_timer") != serialized_fields.end())
	{
		const SerializedField& _timer_sf = serialized_fields["_timer"];
		if (_timer_sf.type_name == "float")
		{
			_timer = std::any_cast<float>(_timer_sf.copy);
		}
	}

	if(serialized_fields.find("_previous_state") != serialized_fields.end())
	{
		const SerializedField& _previous_state_sf = serialized_fields["_previous_state"];
		if (_previous_state_sf.type_name == "PlayerState")
		{
			_previous_state = std::any_cast<PlayerState>(_previous_state_sf.copy);
		}
	}
}

void Hachiko::Scripting::PlayerSoundManager::SerializeTo(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::SerializeTo(serialized_fields);

	serialized_fields["_step_frequency"] = SerializedField(std::string("_step_frequency"), std::make_any<float>(_step_frequency), std::string("float"));

	serialized_fields["_melee_frequency"] = SerializedField(std::string("_melee_frequency"), std::make_any<float>(_melee_frequency), std::string("float"));

	serialized_fields["_ranged_frequency"] = SerializedField(std::string("_ranged_frequency"), std::make_any<float>(_ranged_frequency), std::string("float"));

	serialized_fields["_timer"] = SerializedField(std::string("_timer"), std::make_any<float>(_timer), std::string("float"));

	serialized_fields["_previous_state"] = SerializedField(std::string("_previous_state"), std::make_any<PlayerState>(_previous_state), std::string("PlayerState"));
}