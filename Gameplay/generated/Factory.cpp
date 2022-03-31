#include "scriptingUtil/gameplaypch.h"
#include "generated/Factory.h"
#include "PlayerController.h"


Hachiko::Scripting::Script* InstantiateScript(Hachiko::GameObject* script_owner, const std::string& script_name)
{
	if (script_name == "PlayerController")
	{
		return new Hachiko::Scripting::PlayerController(script_owner);
	}

	return nullptr;
}