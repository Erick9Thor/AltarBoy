#include "scriptingUtil/gameplaypch.h"
#include "SomeScript.h"
#include <Globals.h>

Hachiko::Scripting::SomeScript::SomeScript(GameObject* game_object)
	: Script(game_object, "SomeScript")
	, _counter(0)
	, _int_vector({})
{
}

void Hachiko::Scripting::SomeScript::OnStart()
{
}

void Hachiko::Scripting::SomeScript::OnUpdate()
{
	HE_LOG("SomeScript::OnUpdate");

	_counter++;

	if (_counter % 5 == 0)
	{
		_int_vector.push_back(_counter);

		HE_LOG("HELLO %i", _counter);
	}
}
