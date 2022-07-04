#include "scriptingUtil/gameplaypch.h"
#include "constants/Scenes.h"
#include "entities/Stats.h"
#include "entities/enemies/WormController.h"
#include "entities/player/PlayerController.h"
#include "constants/Sounds.h"

//// TODO: Delete this include:
//#include <modules/ModuleSceneManager.h>

#include <resources/ResourceAnimation.h>
#include <components/ComponentAgent.h>
#include <components/ComponentTransform.h>

#include "misc/AudioManager.h"

Hachiko::Scripting::WormController::WormController(GameObject* game_object)
	: Script(game_object, "WormController")
{
}