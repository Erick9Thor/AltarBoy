#include "scriptingUtil/gameplaypch.h"
#include "generated/Factory.h"
#include "BackToMainMenu.h"
#include "DynamicCamera.h"
#include "EnemyController.h"
#include "MainMenuManager.h"
#include "PlayerAnimationManager.h"
#include "PlayerCamera.h"
#include "PlayerController.h"
#include "PlayerSoundManager.h"


Hachiko::Scripting::Script* InstantiateScript(Hachiko::GameObject* script_owner, const std::string& script_name)
{
	if (script_name == "BackToMainMenu")
	{
		return new Hachiko::Scripting::BackToMainMenu(script_owner);
	}

	if (script_name == "DynamicCamera")
	{
		return new Hachiko::Scripting::DynamicCamera(script_owner);
	}

	if (script_name == "EnemyController")
	{
		return new Hachiko::Scripting::EnemyController(script_owner);
	}

	if (script_name == "MainMenuManager")
	{
		return new Hachiko::Scripting::MainMenuManager(script_owner);
	}

	if (script_name == "PlayerAnimationManager")
	{
		return new Hachiko::Scripting::PlayerAnimationManager(script_owner);
	}

	if (script_name == "PlayerCamera")
	{
		return new Hachiko::Scripting::PlayerCamera(script_owner);
	}

	if (script_name == "PlayerController")
	{
		return new Hachiko::Scripting::PlayerController(script_owner);
	}

	if (script_name == "PlayerSoundManager")
	{
		return new Hachiko::Scripting::PlayerSoundManager(script_owner);
	}

	return nullptr;
}