#include "scriptingUtil/gameplaypch.h"
#include "entities/player/CombatVisualEffectsPool.h"
#include "constants/Scenes.h"

Hachiko::Scripting::CombatVisualEffectsPool::CombatVisualEffectsPool(GameObject* game_object)
	: Script(game_object, "CombatVisualEffectsPool")
	, _billboards({})
	, _current_billboard_index(0)
{
}

void Hachiko::Scripting::CombatVisualEffectsPool::OnAwake()
{
	_billboards.clear();
	_billboards.reserve(15);

	for (ComponentBillboard* billboard : game_object->GetComponentsInDescendants<ComponentBillboard>())
	{
		_billboards.push_back(billboard);
	}
}

void Hachiko::Scripting::CombatVisualEffectsPool::OnUpdate()
{
}

void Hachiko::Scripting::CombatVisualEffectsPool::PlayPlayerAttackEffect(PlayerController::WeaponUsed weapon_type, int attack_index, float3 position)
{
	ComponentBillboard* current_attack_billboard = nullptr;

	switch (weapon_type)
	{
	case PlayerController::WeaponUsed::MELEE:
	{
		current_attack_billboard = _melee_vfx->GetComponent<ComponentBillboard>();
		break;
	}
	case PlayerController::WeaponUsed::CLAW:
	{
		current_attack_billboard = _claw_vfx->GetComponent<ComponentBillboard>();
		break;
	}
	case PlayerController::WeaponUsed::SWORD:
	{
		current_attack_billboard = _sword_vfx->GetComponent<ComponentBillboard>();
		break;
	}
	case PlayerController::WeaponUsed::HAMMER:
	{
		current_attack_billboard = _hammer_vfx->GetComponent<ComponentBillboard>();
		break;
	}
	default:
		break;
	}

	float3 main_camera_direction = (Scenes::GetMainCamera()->GetTransform()->GetGlobalPosition() - position).Normalized() * 1.5f;

	current_attack_billboard->GetGameObject()->GetTransform()->SetGlobalPosition(main_camera_direction + position + float3::unitY * 1.5f);

	current_attack_billboard->Restart();
}

void Hachiko::Scripting::CombatVisualEffectsPool::PlayEnemyAttackEffect(EnemyType _enemy_type, float3 position)
{
	if (_billboards.size() <= 0)
	{
		return;
	}

	_current_billboard_index = _current_billboard_index % _billboards.size();

	ComponentBillboard* current_attack_billboard = _billboards[_current_billboard_index];

	if (current_attack_billboard == nullptr)
	{
		return;
	}

	float3 main_camera_direction = (Scenes::GetMainCamera()->GetTransform()->GetGlobalPosition() - position).Normalized() * 1.5f;

	current_attack_billboard->GetGameObject()->GetTransform()->SetGlobalPosition(main_camera_direction + position + float3::unitY * 1.5f);

	/*switch (_enemy_type)
	{
	case EnemyType::BEETLE:
	{
		break;
	}
	case EnemyType::WORM:
	{
		break;
	}
	default:
		break;
	}*/

	// current_attack_billboard->GetTextureProperties().SetFlip(Hachiko::bool2(false, true));
	
	current_attack_billboard->Restart();

	_current_billboard_index = (_current_billboard_index + 1) % _billboards.size();
}

Hachiko::ComponentBillboard* Hachiko::Scripting::CombatVisualEffectsPool::GetCurrentBillboard() const
{
	return _billboards[_current_billboard_index];
}
