#include "scriptingUtil/gameplaypch.h"
#include "PlayerController.h"
#include <core/GameObject.h>
#include <Globals.h>

Hachiko::Scripting::PlayerController::PlayerController(GameObject* game_object)
	: Script(game_object, "PlayerController")
{}

void Hachiko::Scripting::PlayerController::Start()
{
	HE_LOG("PlayerController::This is experimentation no 0 for Start.");
}

void Hachiko::Scripting::PlayerController::Update()
{
	HE_LOG("PlayerController::This is experimentation no 0 for Update.");
}