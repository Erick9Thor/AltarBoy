#include <vector>
#include <string>
#include <unordered_map>
#include "scriptingUtil/gameplaypch.h"
#include "Funky.h"
#include "PlayerController.h"
#include "SomeScript.h"


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

	if(serialized_fields.find("_integer_field") != serialized_fields.end())
	{
		const SerializedField& _integer_field_sf = serialized_fields["_integer_field"];
		if (_integer_field_sf.type_name == "int")
		{
			_integer_field = std::any_cast<int>(_integer_field_sf.copy);
		}
	}

	if(serialized_fields.find("_string_field") != serialized_fields.end())
	{
		const SerializedField& _string_field_sf = serialized_fields["_string_field"];
		if (_string_field_sf.type_name == "std::string")
		{
			_string_field = std::any_cast<std::string>(_string_field_sf.copy);
		}
	}
}

void Hachiko::Scripting::PlayerController::SerializeTo(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::SerializeTo(serialized_fields);

	serialized_fields["_integer_field"] = SerializedField(std::string("_integer_field"), std::make_any<int>(_integer_field), std::string("int"));

	serialized_fields["_string_field"] = SerializedField(std::string("_string_field"), std::make_any<std::string>(_string_field), std::string("std::string"));
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