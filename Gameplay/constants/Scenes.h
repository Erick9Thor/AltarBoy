#pragma once

#include "Gameplay.h"

namespace Hachiko::Scenes
{
	static UID GAME = 3947210229329588811;
	static UID WIN = 3083261127926411514;
	static UID LOSE = 3083261127926411514;
	static UID MAIN_MENU = 3083261127926411514;
	static const char* player_go_name = "Player";
	static const char* enemies_go_name = "Enemies";
	static const char* level_manager_go_name = "LevelManager";
	static const char* terrain_container_go_name = "Level";

	static GameObject* GetPlayer()
	{
		return SceneManagement::FindInCurrentScene(player_go_name);
	}

	static GameObject* GetEnemiesContainer()
	{
		return SceneManagement::FindInCurrentScene(enemies_go_name);
	}

	static GameObject* GetLevelManager()
	{
		return SceneManagement::FindInCurrentScene(level_manager_go_name);
	}

	static GameObject* GetTerrainContainer()
	{
		return SceneManagement::FindInCurrentScene(terrain_container_go_name);
	}
}