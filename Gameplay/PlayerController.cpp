#include "scriptingUtil/gameplaypch.h"
#include "PlayerController.h"
#include <core/GameObject.h>
#include <Globals.h>

Hachiko::Scripting::PlayerController::PlayerController(GameObject* game_object)
	: Script(game_object, "PlayerController")
	, _integer_field(0)
	, _string_field("")
{}

void Hachiko::Scripting::PlayerController::OnStart()
{
	HE_LOG("PlayerController::This is experimentation no 0 for OnStart.");
}

void Hachiko::Scripting::PlayerController::OnUpdate()
{
	HE_LOG("PlayerController::This is experimentation no 0 for OnUpdate.");

	++_integer_field;
	_string_field = "This is string field";

	HE_LOG("_integer_field: %i _string_field: %s", _integer_field,
		_string_field.c_str());

	HE_LOG("game_object name: %s", game_object->name.c_str());

	PlayerController* player_controller_script = 
		game_object->GetComponent<PlayerController>();

	HE_LOG("HOW ARE YOU RUNNING?");

	HE_LOG("With GetComponent: %s", 
		player_controller_script->_string_field.c_str());
}