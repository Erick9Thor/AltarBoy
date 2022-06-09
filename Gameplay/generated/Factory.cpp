#include "scriptingUtil/gameplaypch.h"
#include "generated/Factory.h"
#include "AudioManager.h"
#include "BackToMainMenu.h"
#include "BulletController.h"
#include "CrystalExplosion.h"
#include "DynamicCamera.h"
#include "EnemyBulletController.h"
#include "EnemyController.h"
#include "FancyLights.h"
#include "MainMenuManager.h"
#include "PlayerCamera.h"
#include "PlayerController.h"
#include "PlayerSoundManager.h"
#include "RoomTeleporter.h"
#include "Stats.h"


Hachiko::Scripting::Script* InstantiateScript(Hachiko::GameObject* script_owner, const std::string& script_name)
{
	if (script_name == "AudioManager")
	{
		return new Hachiko::Scripting::AudioManager(script_owner);
	}

	if (script_name == "BackToMainMenu")
	{
		return new Hachiko::Scripting::BackToMainMenu(script_owner);
	}

	if (script_name == "BulletController")
	{
		return new Hachiko::Scripting::BulletController(script_owner);
	}

	if (script_name == "CrystalExplosion")
	{
		return new Hachiko::Scripting::CrystalExplosion(script_owner);
	}

	if (script_name == "DynamicCamera")
	{
		return new Hachiko::Scripting::DynamicCamera(script_owner);
	}

	if (script_name == "EnemyBulletController")
	{
		return new Hachiko::Scripting::EnemyBulletController(script_owner);
	}

	if (script_name == "EnemyController")
	{
		return new Hachiko::Scripting::EnemyController(script_owner);
	}

	if (script_name == "FancyLights")
	{
		return new Hachiko::Scripting::FancyLights(script_owner);
	}

	if (script_name == "MainMenuManager")
	{
		return new Hachiko::Scripting::MainMenuManager(script_owner);
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

	if (script_name == "Stats")
	{
		return new Hachiko::Scripting::Stats(script_owner);
	}

	return nullptr;
}