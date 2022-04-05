#include "scriptingUtil/gameplaypch.h"
#include "Experiment.h"

Hachiko::Scripting::Experiment::Experiment(GameObject* game_object)
	: Script(game_object, "Experiment")
{
}

void Hachiko::Scripting::Experiment::OnStart()
{
}

void Hachiko::Scripting::Experiment::OnUpdate()
{
	math::float2 mouse_position = Input::GetMousePosition();

	HE_LOG("Mouse Position: %f, %f", mouse_position.x, mouse_position.y);
}