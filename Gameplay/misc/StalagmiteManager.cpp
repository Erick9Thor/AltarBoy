#include "scriptingUtil/gameplaypch.h"
#include "StalagmiteManager.h"


Hachiko::Scripting::StalagmiteManager::StalagmiteManager(GameObject* game_object)
	: Script(game_object, "StalagmiteManager")
	, _spawn_cooldown(5.0f)
	, _go_stalagmite(nullptr)
{
}

void Hachiko::Scripting::StalagmiteManager::OnStart()
{
	GenerateStalagmites();
}

void Hachiko::Scripting::StalagmiteManager::GenerateStalagmites()
{
}

void Hachiko::Scripting::StalagmiteManager::OnUpdate()
{
	for (Stalagmite* stalagmite : _stalagmites) {
		if (stalagmite->stalagmite_state == StalagmiteState::INVALID)
		{
			// LOOK cooldown and active if an
		}
		else {
			if (stalagmite->stalagmite_state == StalagmiteState::FALLING_AREA)
			{
				
			}
			else if (stalagmite->stalagmite_state == StalagmiteState::COLLAPSE)
			{
				
			}
			else if (stalagmite->stalagmite_state == StalagmiteState::SPAWN_CRYSTAL)
			{

			}

			UpdateStalagmiteState(stalagmite);
		}
	}
}

void Hachiko::Scripting::StalagmiteManager::UpdateStalagmiteState(Stalagmite* stalagmite)
{
	StalagmiteState platform_state = stalagmite->stalagmite_state;
	bool state_changed = platform_state != stalagmite->_previous_state;

	if (!state_changed)
	{
		return;
	}

	stalagmite->_previous_state = platform_state;

	switch (platform_state)
	{
	case StalagmiteState::INVALID:
		return;
	case StalagmiteState::FALLING_AREA:
		break;
	case StalagmiteState::COLLAPSE:
		break;
	case StalagmiteState::SPAWN_CRYSTAL:
		break;
	default:
		break;
	}
}


