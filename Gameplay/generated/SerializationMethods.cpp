#include <vector>
#include <string>
#include <unordered_map>
#include "scriptingUtil/gameplaypch.h"
#include "PlayerController.h"


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