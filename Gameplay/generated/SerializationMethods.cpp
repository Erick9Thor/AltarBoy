#include <vector>
#include <string>
#include <unordered_map>
#include "scriptingUtil/gameplaypch.h"
#include "Experiment.h"
#include "Funky.h"
#include "PlayerController.h"
#include "SomeScript.h"


void Hachiko::Scripting::Experiment::DeserializeFrom(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::DeserializeFrom(serialized_fields);
}

void Hachiko::Scripting::Experiment::SerializeTo(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::SerializeTo(serialized_fields);
}

void Hachiko::Scripting::Funky::DeserializeFrom(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::DeserializeFrom(serialized_fields);

	if(serialized_fields.find("_lerp_index") != serialized_fields.end())
	{
		const SerializedField& _lerp_index_sf = serialized_fields["_lerp_index"];
		if (_lerp_index_sf.type_name == "int")
		{
			_lerp_index = std::any_cast<int>(_lerp_index_sf.copy);
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

	if(serialized_fields.find("_lerp_reverse") != serialized_fields.end())
	{
		const SerializedField& _lerp_reverse_sf = serialized_fields["_lerp_reverse"];
		if (_lerp_reverse_sf.type_name == "bool")
		{
			_lerp_reverse = std::any_cast<bool>(_lerp_reverse_sf.copy);
		}
	}

	if(serialized_fields.find("_initial_color") != serialized_fields.end())
	{
		const SerializedField& _initial_color_sf = serialized_fields["_initial_color"];
		if (_initial_color_sf.type_name == "math::float4")
		{
			_initial_color = std::any_cast<math::float4>(_initial_color_sf.copy);
		}
	}
}

void Hachiko::Scripting::Funky::SerializeTo(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::SerializeTo(serialized_fields);

	serialized_fields["_lerp_index"] = SerializedField(std::string("_lerp_index"), std::make_any<int>(_lerp_index), std::string("int"));

	serialized_fields["_lerp_position"] = SerializedField(std::string("_lerp_position"), std::make_any<float>(_lerp_position), std::string("float"));

	serialized_fields["_lerp_reverse"] = SerializedField(std::string("_lerp_reverse"), std::make_any<bool>(_lerp_reverse), std::string("bool"));

	serialized_fields["_initial_color"] = SerializedField(std::string("_initial_color"), std::make_any<math::float4>(_initial_color), std::string("math::float4"));
}

void Hachiko::Scripting::PlayerController::DeserializeFrom(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::DeserializeFrom(serialized_fields);

	if(serialized_fields.find("_movement_speed") != serialized_fields.end())
	{
		const SerializedField& _movement_speed_sf = serialized_fields["_movement_speed"];
		if (_movement_speed_sf.type_name == "float")
		{
			_movement_speed = std::any_cast<float>(_movement_speed_sf.copy);
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

	if(serialized_fields.find("_is_dashing") != serialized_fields.end())
	{
		const SerializedField& _is_dashing_sf = serialized_fields["_is_dashing"];
		if (_is_dashing_sf.type_name == "bool")
		{
			_is_dashing = std::any_cast<bool>(_is_dashing_sf.copy);
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

	if(serialized_fields.find("_dash_start") != serialized_fields.end())
	{
		const SerializedField& _dash_start_sf = serialized_fields["_dash_start"];
		if (_dash_start_sf.type_name == "math::float3")
		{
			_dash_start = std::any_cast<math::float3>(_dash_start_sf.copy);
		}
	}

	if(serialized_fields.find("_rotation_speed") != serialized_fields.end())
	{
		const SerializedField& _rotation_speed_sf = serialized_fields["_rotation_speed"];
		if (_rotation_speed_sf.type_name == "float")
		{
			_rotation_speed = std::any_cast<float>(_rotation_speed_sf.copy);
		}
	}
}

void Hachiko::Scripting::PlayerController::SerializeTo(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::SerializeTo(serialized_fields);

	serialized_fields["_movement_speed"] = SerializedField(std::string("_movement_speed"), std::make_any<float>(_movement_speed), std::string("float"));

	serialized_fields["_dash_duration"] = SerializedField(std::string("_dash_duration"), std::make_any<float>(_dash_duration), std::string("float"));

	serialized_fields["_dash_distance"] = SerializedField(std::string("_dash_distance"), std::make_any<float>(_dash_distance), std::string("float"));

	serialized_fields["_is_dashing"] = SerializedField(std::string("_is_dashing"), std::make_any<bool>(_is_dashing), std::string("bool"));

	serialized_fields["_dash_progress"] = SerializedField(std::string("_dash_progress"), std::make_any<float>(_dash_progress), std::string("float"));

	serialized_fields["_dash_start"] = SerializedField(std::string("_dash_start"), std::make_any<math::float3>(_dash_start), std::string("math::float3"));

	serialized_fields["_rotation_speed"] = SerializedField(std::string("_rotation_speed"), std::make_any<float>(_rotation_speed), std::string("float"));
}

void Hachiko::Scripting::SomeScript::DeserializeFrom(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::DeserializeFrom(serialized_fields);

	if(serialized_fields.find("_initial_position") != serialized_fields.end())
	{
		const SerializedField& _initial_position_sf = serialized_fields["_initial_position"];
		if (_initial_position_sf.type_name == "math::float3")
		{
			_initial_position = std::any_cast<math::float3>(_initial_position_sf.copy);
		}
	}

	if(serialized_fields.find("_position_offset") != serialized_fields.end())
	{
		const SerializedField& _position_offset_sf = serialized_fields["_position_offset"];
		if (_position_offset_sf.type_name == "math::float3")
		{
			_position_offset = std::any_cast<math::float3>(_position_offset_sf.copy);
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

void Hachiko::Scripting::SomeScript::SerializeTo(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::SerializeTo(serialized_fields);

	serialized_fields["_initial_position"] = SerializedField(std::string("_initial_position"), std::make_any<math::float3>(_initial_position), std::string("math::float3"));

	serialized_fields["_position_offset"] = SerializedField(std::string("_position_offset"), std::make_any<math::float3>(_position_offset), std::string("math::float3"));

	serialized_fields["_lerp_position"] = SerializedField(std::string("_lerp_position"), std::make_any<float>(_lerp_position), std::string("float"));
}