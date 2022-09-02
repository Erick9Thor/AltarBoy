#include "scriptingUtil/gameplaypch.h"

#include "entities/enemies/BossController.h"
#include "entities/Stats.h"
#include "constants/Scenes.h"



Hachiko::Scripting::BossController::BossController(GameObject* game_object)
	: Script(game_object, "BossController")
	, state_value(0)
	, hp_bar_go(nullptr)
{
	
}

void Hachiko::Scripting::BossController::OnAwake()
{
	SetUpHpBar();
}

void Hachiko::Scripting::BossController::OnStart()
{
}

void Hachiko::Scripting::BossController::OnUpdate()
{
	if (Input::IsKeyDown(Input::KeyCode::KEY_H))
	{
		constexpr int player_dmg = 5;
		RegisterHit(player_dmg);
	}
}

void Hachiko::Scripting::BossController::RegisterHit(int dmg)
{
	current_hp -= dmg;
	UpdateHpBar();
}

void Hachiko::Scripting::BossController::UpdateHpBar()
{
	if (hp_bar)
	{
		hp_bar->SetFilledValue(current_hp);
	}
}

void Hachiko::Scripting::BossController::SetUpHpBar()
{
	if (hp_bar_go)
	{
		hp_bar = hp_bar_go->GetComponent<ComponentProgressBar>();
	}
	if (hp_bar)
	{
		hp_bar->SetMax(total_hp);
		hp_bar->SetMin(0);
		UpdateHpBar();
	}
}
