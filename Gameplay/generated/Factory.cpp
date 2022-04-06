#include "scriptingUtil/gameplaypch.h"
#include "generated/Factory.h"
#include "DynamicCamera.h"
#include "Experiment.h"
#include "Funky.h"
#include "PlayerController.h"
#include "SomeScript.h"


Hachiko::Scripting::Script* InstantiateScript(Hachiko::GameObject* script_owner, const std::string& script_name)
{
	if (script_name == "DynamicCamera")
	{
		return new Hachiko::Scripting::DynamicCamera(script_owner);
	}

	if (script_name == "Experiment")
	{
		return new Hachiko::Scripting::Experiment(script_owner);
	}

	if (script_name == "Funky")
	{
		return new Hachiko::Scripting::Funky(script_owner);
	}

	if (script_name == "PlayerController")
	{
		return new Hachiko::Scripting::PlayerController(script_owner);
	}

	if (script_name == "SomeScript")
	{
		return new Hachiko::Scripting::SomeScript(script_owner);
	}

	return nullptr;
}