#include "scriptingUtil/gameplaypch.h"

#include "GauntletManager.h"

Hachiko::Scripting::GauntletManager::GauntletManager(GameObject* game_object)
	: Script(game_object, "GauntletManager")
	, _trigger_radius(0.f)
	, _door_1(nullptr)
	, _door_2(nullptr)
	, _pack_1(nullptr)
	, _pack_2(nullptr)
	, _pack_3(nullptr)
{}

void Hachiko::Scripting::GauntletManager::OnAwake()
{
	_combat_manager = _combat_manager_go->GetComponent<CombatManager>();
	
	_doors.clear();
	if (_door_1 && _door_1->GetComponent<DoorController>()) _doors.push_back(_door_1->GetComponent<DoorController>());
	if (_door_2 && _door_2->GetComponent<DoorController>()) _doors.push_back(_door_2->GetComponent<DoorController>());
	
	_enemy_packs.clear();
	if (_pack_1) _enemy_packs.push_back(_pack_1);
	if (_pack_2) _enemy_packs.push_back(_pack_2);
	if (_pack_3) _enemy_packs.push_back(_pack_3);	

}

void Hachiko::Scripting::GauntletManager::OnStart()
{
	ResetGauntlet();

	if (!_use_trigger)
	{
		StartGauntlet();
	}
}

void Hachiko::Scripting::GauntletManager::OnUpdate()
{
	if (completed) return;
	
	if (!started)
	{
		if (_trigger_radius >= game_object->GetTransform()->GetGlobalPosition().Distance(_combat_manager->GetPlayer()->GetTransform()->GetGlobalPosition()))
		{
			// Here we would start spawning the first round
			StartGauntlet();
		}
	}
	else
	{
		CheckRoundStatus();
	}

}

void Hachiko::Scripting::GauntletManager::StartGauntlet()
{
	started = true;
	current_round = 0;
	SpawnRound(current_round);
}

void Hachiko::Scripting::GauntletManager::ResetGauntlet()
{
	CloseDoors();
	for (GameObject* enemy_pack : _enemy_packs)
	{
		_combat_manager->DeactivateEnemyPack(enemy_pack);
	}
	current_round = 0;
}

void Hachiko::Scripting::GauntletManager::CheckRoundStatus()
{
	if (current_round >= _enemy_packs.size()) {
		completed = true;
		OpenDoors();
		return;
	}

	if(!_combat_manager->IsPackDead(_enemy_packs[current_round])) return;
	++current_round;
	SpawnRound(current_round);
}

void Hachiko::Scripting::GauntletManager::OpenDoors()
{
	for (DoorController* door : _doors)
	{
		door->Open();
	}
}

void Hachiko::Scripting::GauntletManager::CloseDoors()
{
	for (DoorController* door : _doors)
	{
		door->Close();
	}
}

void Hachiko::Scripting::GauntletManager::SpawnRound(unsigned round)
{
	if (round >= _enemy_packs.size()) return;
	_combat_manager->ResetEnemyPack(_enemy_packs[round]);
	_combat_manager->ActivateEnemyPack(_enemy_packs[round]);
}
