#include "core/hepch.h"
#include "scripting/Script.h"
#include "core/GameObject.h"

Hachiko::Scripting::Script::Script(GameObject* new_game_object, 
	std::string new_name) 
	: Component(Hachiko::Component::Type::SCRIPT, new_game_object)
	, ISerializable()
	, name(new_name) 
{
	active = true;
}

const std::string& Hachiko::Scripting::Script::GetName() const 
{
	return name;
}

void Hachiko::Scripting::Script::SerializeTo(std::unordered_map<std::string, 
	SerializedField>& serialized_fields)
{
    // For field game_object : GameObject
	serialized_fields["game_object"] = SerializedField(
		std::string("game_object"), 
		std::make_any<GameObject*>(game_object), 
		std::string("GameObject*"));

	// For field active : bool
	serialized_fields["active"] = SerializedField(
		std::string("active"), 
		std::make_any<bool>(active), 
		std::string("bool"));

    // For field uid : UID
	serialized_fields["uid"] = SerializedField(
		std::string("uid"), 
		std::make_any<UID>(GetID()), 
		std::string("UID"));
}

void Hachiko::Scripting::Script::DeserializeFrom(
	std::unordered_map<std::string, SerializedField>& serialized_fields)
{
	if (serialized_fields.empty())
	{
        return;
	}

	// For field game_object : GameObject
    game_object = 
		std::any_cast<GameObject*>(serialized_fields["game_object"].copy);

	// For field active : bool
	active = std::any_cast<bool>(serialized_fields["active"].copy);

	// For field uid : UID
    OverrideUID(std::any_cast<UID>(serialized_fields["uid"].copy));
}