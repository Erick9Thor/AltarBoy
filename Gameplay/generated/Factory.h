#pragma once

#include "scriptingUtil/framework.h"
#include "core/GameObject.h"
#include "scripting/Script.h"

extern "C" GAMEPLAY_API Hachiko::Scripting::Script* __cdecl 
InstantiateScript(Hachiko::GameObject* game_object, 
	const std::string & script_name);
