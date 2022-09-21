#include "scriptingUtil/gameplaypch.h"
#include "constants/Scenes.h"

#include "StalagmiteManager.h"


Hachiko::Scripting::StalagmiteManager::StalagmiteManager(GameObject* game_object)
	: Script(game_object, "StalagmiteManager")
	, _falling_time(0.4f)
	, _crystal_parent(nullptr)
{
}

void Hachiko::Scripting::StalagmiteManager::OnAwake()
{
	_player_camera = Scenes::GetMainCamera()->GetComponent<PlayerCamera>();
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
			_player_camera->Shake(0.5f, 0.8f);
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
		stalagmite->ActiveStalagmite();
		stalagmite->ActiveEffects();
		break;
	case StalagmiteState::SPAWN_CRYSTAL:
		stalagmite->SetCrystalParent(_crystal_parent);
		stalagmite->ActiveCrystal();
		break;
	default:
		break;
	}
}

void Hachiko::Scripting::StalagmiteManager::FallingStalagmite(Stalagmite* stalagmite)
{
	stalagmite->Falling();
}


