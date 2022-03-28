#include "core/hepch.h"
#include "scripting/Script.h"
#include "core/GameObject.h"

Hachiko::Scripting::Script::Script(GameObject* new_game_object, 
	std::string new_name) 
	: game_object(new_game_object) 
	, name(new_name) 
{
}

const std::string& Hachiko::Scripting::Script::GetName() const 
{
	return name;
}

void Hachiko::Scripting::Script::SerializeTo(std::unordered_map<std::string, 
	SerializedField>& serialized_fields)
{
	serialized_fields["game_object"] = SerializedField(
		std::string("game_object"), 
		std::make_any<GameObject*>(game_object), 
		std::string("GameObject*"));
}

void Hachiko::Scripting::Script::DeserializeFrom(
	std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	if (serialized_fields.find("game_object") != serialized_fields.end())
	{
		const SerializedField& game_object_serialized_field = 
			serialized_fields["game_object"];

		if (game_object_serialized_field.type_name == "GameObject*")
		{
			game_object = std::any_cast<GameObject*>(
				game_object_serialized_field.copy);
		}
	}
}