#include "scriptingUtil/gameplaypch.h"

#include "LevelManager.h"

Hachiko::Scripting::LevelManager::LevelManager(GameObject* game_object)
	: Script(game_object, "LevelManager")
	, _respawn_position(float3::zero)
	, _last_gauntlet(nullptr)
{}