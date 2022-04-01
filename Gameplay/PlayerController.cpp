#include "scriptingUtil/gameplaypch.h"
#include "PlayerController.h"
#include <core/GameObject.h>
#include <Globals.h>

Hachiko::Scripting::PlayerController::PlayerController(GameObject* game_object)
	: Script(game_object, "PlayerController")
	, _integer_field(0)
	, _string_field("")
{}

void Hachiko::Scripting::PlayerController::Start()
{
	HE_LOG("PlayerController::This is experimentation no 0 for Start.");
}

void Hachiko::Scripting::PlayerController::Update()
{
	HE_LOG("PlayerController::This is experimentation no 0 for Update.");

	++_integer_field;
	_string_field = "This is string field";

	HE_LOG("_integer_field: %i _string_field: %s", _integer_field,
		_string_field.c_str());

	HE_LOG("game_object name: %s", game_object->name.c_str());
}