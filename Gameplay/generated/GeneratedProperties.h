#pragma once

#include "scriptingUtil/framework.h"

extern "C" const GAMEPLAY_API size_t __cdecl SCRIPT_COUNT = 31;

extern "C" const GAMEPLAY_API char* __cdecl SCRIPT_NAMES[SCRIPT_COUNT] =
{
	"TriggerAnim",
	"Stats",
	"CrystalExplosion",
	"BossController",
	"BugAnimationManager",
	"EnemyController",
	"CombatManager",
	"CombatVisualEffectsPool",
	"PlayerAnimationManager",
	"PlayerCamera",
	"PlayerController",
	"PlayerSoundManager",
	"RoomTeleporter",
	"AssetsObstacle",
	"AudioManager",
	"BlinkingLight",
	"BossLaserController",
	"CameraPosChange",
	"CrystalPlatform",
	"DoorController",
	"DynamicCamera",
	"FancyLights",
	"GauntletManager",
	"LaserController",
	"LevelManager",
	"PillarCheckpoint",
	"Spawner",
	"TimeManager",
	"BackToMainMenu",
	"DebugManager",
	"MainMenuManager"
};
