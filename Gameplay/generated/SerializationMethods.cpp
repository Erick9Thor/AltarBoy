#include <vector>
#include <string>
#include <unordered_map>
#include "scriptingUtil/gameplaypch.h"
#include "PlayerController.h"
#include "SomeScript.h"


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

	if(serialized_fields.find("_int_vector") != serialized_fields.end())
	{
		const SerializedField& _int_vector_sf = serialized_fields["_int_vector"];
		if (_int_vector_sf.type_name == "std::vector<int>")
		{
			_int_vector = std::any_cast<std::vector<int>>(_int_vector_sf.copy);
		}
	}

	if(serialized_fields.find("_counter") != serialized_fields.end())
	{
		const SerializedField& _counter_sf = serialized_fields["_counter"];
		if (_counter_sf.type_name == "int")
		{
			_counter = std::any_cast<int>(_counter_sf.copy);
		}
	}
}

void Hachiko::Scripting::SomeScript::SerializeTo(std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	Hachiko::Scripting::Script::SerializeTo(serialized_fields);

	serialized_fields["_int_vector"] = SerializedField(std::string("_int_vector"), std::make_any<std::vector<int>>(_int_vector), std::string("std::vector<int>"));

	serialized_fields["_counter"] = SerializedField(std::string("_counter"), std::make_any<int>(_counter), std::string("int"));
}