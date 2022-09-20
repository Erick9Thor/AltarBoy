#include "scriptingUtil/gameplaypch.h"
#include "StalagmiteManager.h"


Hachiko::Scripting::StalagmiteManager::StalagmiteManager(GameObject* game_object)
	: Script(game_object, "StalagmiteManager")
	, _falling_time(0.4f)
	, _falling_cooldown(0.5f)
{
}

void Hachiko::Scripting::StalagmiteManager::OnStart()
{
	GenerateStalagmites();
}

void Hachiko::Scripting::StalagmiteManager::GenerateStalagmites()
{
	std::vector<GameObject*> stalagmites_children = game_object->children;

	for (int i = 0; i < stalagmites_children.size(); i++)
	{
		Stalagmite* stalagmite_one = stalagmites_children[i]->GetComponent<Stalagmite>();
		_stalagmites.push_back(stalagmite_one);
	}
}

void Hachiko::Scripting::StalagmiteManager::OnUpdate()
{
	for (unsigned i = 0; i < _stalagmites.size(); i++)
	{
		if (_stalagmites[i]->GetState() == StalagmiteState::INVALID)
		{
			cooldown_elapsed += Time::DeltaTime();
			if (cooldown_elapsed >= _falling_cooldown)
			{
				_stalagmites[i]->SetNewState(StalagmiteState::FALLING);
			}
		}
		else {
			if (_stalagmites[i]->GetState() == StalagmiteState::FALLING)
			{
				falling_elapsed += Time::DeltaTime();
				if (falling_elapsed >= _falling_time)
				{
					_stalagmites[i]->SetNewState(StalagmiteState::SPAWN_CRYSTAL);
				}

				FallingStalagmite(_stalagmites[i]);
			}
			else if (_stalagmites[i]->GetState() == StalagmiteState::SPAWN_CRYSTAL)
			{

			}

			UpdateStalagmiteState(_stalagmites[i]);
		}
	}
}

void Hachiko::Scripting::StalagmiteManager::UpdateStalagmiteState(Stalagmite* stalagmite)
{
	StalagmiteState platform_state = stalagmite->GetState();
	bool state_changed = platform_state != stalagmite->GetPreviousState();

	if (!state_changed)
	{
		return;
	}

	stalagmite->SetPreviousState(platform_state);

	switch (platform_state)
	{
	case StalagmiteState::INVALID:
		return;
	case StalagmiteState::FALLING:
		break;
	case StalagmiteState::SPAWN_CRYSTAL:
		break;
	default:
		break;
	}
}

void Hachiko::Scripting::StalagmiteManager::FallingStalagmite(Stalagmite* stalagmite)
{
	stalagmite->Falling();
}


