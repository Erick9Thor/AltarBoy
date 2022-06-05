#include "scriptingUtil/gameplaypch.h"
#include "generated/Factory.h"
#include "BackToMainMenu.h"
#include "BugAnimationManager.h"
#include "CrystalExplosion.h"
#include "DynamicCamera.h"
#include "EnemyController.h"
#include "MainMenuManager.h"
#include "PlayerAnimationManager.h"
#include "PlayerCamera.h"
#include "PlayerController.h"
#include "PlayerSoundManager.h"
#include "RoomTeleporter.h"


Hachiko::Scripting::Script* InstantiateScript(Hachiko::GameObject* script_owner, const std::string& script_name)
{
	if (script_name == "BackToMainMenu")
	{
		return new Hachiko::Scripting::BackToMainMenu(script_owner);
	}

	if (script_name == "BugAnimationManager")
	{
		return new Hachiko::Scripting::BugAnimationManager(script_owner);
	}

	if (script_name == "CrystalExplosion")
	{
		return new Hachiko::Scripting::CrystalExplosion(script_owner);
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

	if (script_name == "RoomTeleporter")
	{
		return new Hachiko::Scripting::RoomTeleporter(script_owner);
	}

	return nullptr;
}