#include "scriptingUtil/gameplaypch.h"

#include "entities/enemies/BossController.h"
#include "entities/enemies/EnemyController.h"
#include "entities/Stats.h"
#include "constants/Scenes.h"

Hachiko::Scripting::BossController::BossController(GameObject* game_object)
	: Script(game_object, "BossController")
	, state_value(0)
	, hp_bar_go(nullptr)
	, cocoon_placeholder_go(nullptr)
	, enemy_pool(nullptr)
	, time_between_enemies(5.0)
	, _current_index_crystals(0)
	, _explosive_crystals({})
{
}

void Hachiko::Scripting::BossController::OnAwake()
{
	player = Scenes::GetPlayer();
	level_manager = Scenes::GetLevelManager()->GetComponent<LevelManager>();
	player_camera = Scenes::GetMainCamera()->GetComponent<PlayerCamera>();
	combat_manager = Scenes::GetCombatManager()->GetComponent<CombatManager>();
	transform = game_object->GetTransform();
	combat_stats = game_object->GetComponent<Stats>();
	agent = game_object->GetComponent<ComponentAgent>();
	agent->SetMaxSpeed(combat_stats->_move_speed);
	SetUpHpBar();
	// Hp bar starts hidden untill encounter starts
	SetHpBarActive(false);
	if (cocoon_placeholder_go)
	{
		cocoon_placeholder_go->SetActive(false);
	}
	gauntlet = gauntlet_go->GetComponent<GauntletManager>();
	
	if (enemy_pool)
	{
		for (GameObject* enemy : enemy_pool->children)
		{
			enemies.push_back(enemy->GetComponent<EnemyController>());
		}
		ResetEnemies();
	}

	_explosive_crystals.clear();
	_explosive_crystals.reserve(5);

	for (GameObject* crystal_go : crystal_pool->children)
	{
		_explosive_crystals.push_back(crystal_go);
	}
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

	if (attack_current_cd > 0.0f)
	{
		attack_current_cd -= Time::DeltaTime();
	}

	StateController();
	state_value = static_cast<int>(state);
}

bool Hachiko::Scripting::BossController::IsAlive() const
{
	return combat_stats->IsAlive();
}

void Hachiko::Scripting::BossController::RegisterHit(int dmg)
{
	if (hitable)
	{
		combat_stats->_current_hp -= dmg;
		UpdateHpBar();

		game_object->ChangeEmissiveColor(float4(255, 255, 255, 255), 0.3f, true);
	}
}

void Hachiko::Scripting::BossController::UpdateHpBar()
{
	if (hp_bar)
	{
		hp_bar->SetFilledValue(combat_stats->_current_hp);
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
		hp_bar->SetMax(combat_stats->_max_hp);
		hp_bar->SetMin(0);
		UpdateHpBar();
	}
}

void Hachiko::Scripting::BossController::SetHpBarActive(bool v)
{
	if (hp_bar_go)
	{
		hp_bar_go->SetActive(v);
	}
}

void Hachiko::Scripting::BossController::StateController()
{
	StateTransitionController();

	switch (state)
	{
	case BossState::WAITING_ENCOUNTER:
		WaitingController();
		break;
	case BossState::STARTING_ENCOUNTER:
		StartEncounterController();
		break;
	case BossState::COMBAT_FORM:
		CombatController();
		break;
	case BossState::CACOON_FORM:
		CacoonController();
		break;
	case BossState::DEAD:
		break;
	}
}

void Hachiko::Scripting::BossController::StateTransitionController()
{
	bool state_changed = state != prev_state;

	if (!state_changed)
	{
		return;
	}

	switch (state)
	{
	case BossState::WAITING_ENCOUNTER:
		// We already start in this state, no transition for now
		// We can reset the scene on death here which will be easier
		break;
	case BossState::STARTING_ENCOUNTER:
		StartEncounter();
		break;
	case BossState::COMBAT_FORM:
		ResetCombatState();
		break;
	case BossState::CACOON_FORM:
		StartCacoon();
		break;
	case BossState::DEAD:
		break;
	}

	prev_state = state;
}

void Hachiko::Scripting::BossController::CombatController()
{
	if (CacoonTrigger())
	{
		state = BossState::CACOON_FORM;
		return;
	}

	if (combat_stats->_current_hp <= 0)
	{
		level_manager->GoalReached();
		state = BossState::DEAD;
		return;
	}
	
	SpawnEnemy();

	CombatTransitionController();

	switch (combat_state)
	{
	case CombatState::IDLE:
		break;
	case CombatState::CHASING:
		ChaseController();
		break;
	case CombatState::ATTACKING:
		MeleeAttackController();
		break;
	case CombatState::SPAWNING_CRYSTALS:
		SpawnCrystalsController();
		break;
	case CombatState::CONSUMING_PARASYTES:
		ConsumeParasytesController();
		break;
	case CombatState::CRYSTAL_JUMP:
		// Do not implement for now
		break;
	}
}

void Hachiko::Scripting::BossController::CombatTransitionController()
{
	bool state_changed = combat_state != prev_combat_state;

	if (!state_changed)
	{
		return;
	}

	switch (combat_state)
	{
	case CombatState::IDLE:
		Chase();
		break;
	case CombatState::CHASING:
		break;
	case CombatState::ATTACKING:
		MeleeAttack();
		break;
	case CombatState::SPAWNING_CRYSTALS:
		SpawnCrystals();
		break;
	case CombatState::CONSUMING_PARASYTES:
		ConsumeParasytes();
		break;
	case CombatState::CRYSTAL_JUMP:
		// Do not implement for now
		break;
	}

	prev_combat_state = combat_state;
}

float Hachiko::Scripting::BossController::GetPlayerDistance()
{
	const float3& player_position = player->GetTransform()->GetGlobalPosition();

	// If player is very cloose change to attack mode
	return transform->GetGlobalPosition().Distance(player_position);
}


void Hachiko::Scripting::BossController::WaitingController()
{
	if (GetPlayerDistance() <= start_encounter_range)
	{
		// Will trigger StartEncounter
		state = BossState::STARTING_ENCOUNTER;
	}
}

void Hachiko::Scripting::BossController::StartEncounter()
{
	SetHpBarActive(true);
}

void Hachiko::Scripting::BossController::StartEncounterController()
{
	// If there is any start sequence manage it here and hold state untill it ends
	// For now it goes to next state instantly
	state = BossState::COMBAT_FORM;
}

void Hachiko::Scripting::BossController::ResetCombatState()
{
	// Reset combat related variables to prevent bugs
	combat_state = CombatState::IDLE;
	prev_combat_state = CombatState::CRYSTAL_JUMP;
}

void Hachiko::Scripting::BossController::Die()
{
	// Start death
}

void Hachiko::Scripting::BossController::DieController()
{
}

void Hachiko::Scripting::BossController::StartCacoon()
{
	hitable = false;
	time_elapse = 0.0;
	if (cocoon_placeholder_go)
	{
		cocoon_placeholder_go->SetActive(true);
	}
	FocusCamera(true);
}

void Hachiko::Scripting::BossController::CacoonController()
{
	// Handle camera focus
	if (camera_focus_on_boss)
	{
		time_elapse += Time::DeltaTime();

		if (time_elapse >= 2) // HARDCODED TIME FOR CAMERA FOCUS
		{
			FocusCamera(false);
		}
	}

	// Replace with is gauntlet completed
	if (gauntlet->IsFinished() || Input::IsKeyDown(Input::KeyCode::KEY_J))
	{
		FinishCacoon();
	}
}

bool Hachiko::Scripting::BossController::CacoonTrigger()
{
	if (gauntlet_thresholds_percent.empty())
	{
		return false;
	}
	if (gauntlet_thresholds_percent.back() >= static_cast<float>(combat_stats->_current_hp) / combat_stats->_max_hp)
	{
		gauntlet_thresholds_percent.pop_back();
		gauntlet->StartGauntlet();
		return true;
	}
	return false;
}

void Hachiko::Scripting::BossController::FinishCacoon()
{
	if (state != BossState::CACOON_FORM)
	{
		return;
	}
	hitable = true;
	if (cocoon_placeholder_go)
	{
		cocoon_placeholder_go->SetActive(false);
	}
	gauntlet->ResetGauntlet();
	state = BossState::COMBAT_FORM;
}

void Hachiko::Scripting::BossController::Chase()
{
	combat_state = CombatState::CHASING;
}

void Hachiko::Scripting::BossController::ChaseController()
{
	const float3& player_position = player->GetTransform()->GetGlobalPosition();

	// If player is very cloose change to attack mode
	float player_distance = transform->GetGlobalPosition().Distance(player_position);

	if (attack_current_cd <= 0.0f && player_distance <= combat_stats->_attack_range) {
		// We expect transition to attack to start the attack
		combat_state = CombatState::ATTACKING;
		attack_current_cd = combat_stats->_attack_cd;
		return;
	}

	transform->LookAtTarget(player_position);

	float3 moving_position;
	// If the boss can not attack it will try to keep some distance
	if (attack_current_cd > 0.0f && player_distance < 8.f)
	{
		// Back down if too close
		if (player_distance < 6.f)
		{
			moving_position = transform->GetGlobalPosition() - transform->GetFront();
			agent->SetMaxSpeed(2.0f);
		}
		// Dont move if already far enough
		else
		{
			return;
		}
	}
	// If not close enough to attack keep chasing
	else
	{
		moving_position = player_position;
		agent->SetMaxSpeed(combat_stats->_move_speed);
	}

	float3 corrected_position = Navigation::GetCorrectedPosition(moving_position, math::float3(10.0f, 10.0f, 10.0f));
	if (corrected_position.x < FLT_MAX)
	{
		target_position = corrected_position;
		transform->LookAtTarget(target_position);
		ComponentAgent* agc = game_object->GetComponent<ComponentAgent>();
		agent->SetTargetPosition(target_position);
	}
}

void Hachiko::Scripting::BossController::MeleeAttack()
{
}

void Hachiko::Scripting::BossController::MeleeAttackController()
{
	// Enemy like attacks for alpha testing, this should be changed
	// It deals no damage also
	CombatManager::AttackStats attack_stats;
	attack_stats.damage = 0;
	attack_stats.knockback_distance = 0.0f;
	attack_stats.width = 4.f;
	attack_stats.range = combat_stats->_attack_range * 1.3f;
	attack_stats.type = CombatManager::AttackType::RECTANGLE;

	float3 emitter_direction = transform->GetFront().Normalized();
	float3 emitter_position = transform->GetGlobalPosition() + emitter_direction * (combat_stats->_attack_range / 2.f);
	float4x4 emitter = float4x4::FromTRS(emitter_position, transform->GetGlobalRotation(), transform->GetGlobalScale());
	Debug::DebugDraw(combat_manager->CreateAttackHitbox(emitter, attack_stats), float3(1.0f, 1.0f, 0.0f));

	combat_manager->EnemyMeleeAttack(emitter, attack_stats);

	//combat_state = CombatState::IDLE;
	combat_state = CombatState::SPAWNING_CRYSTALS;
}

void Hachiko::Scripting::BossController::SpawnCrystals()
{
}

void Hachiko::Scripting::BossController::SpawnCrystalsController()
{
	if (_explosive_crystals.size() <= 0)
	{
		return;
	}

	_current_index_crystals = _current_index_crystals % _explosive_crystals.size();

	GameObject* current_crystal_to_spawn = _explosive_crystals[_current_index_crystals];
	
	if (current_crystal_to_spawn == nullptr)
	{
		return;
	}

	float3 emitter_direction = transform->GetFront().Normalized();
	float random = float(rand() % 10);
	float3 emitter_position = transform->GetGlobalPosition() + emitter_direction * random;

	current_crystal_to_spawn->FindDescendantWithName("ExplosionIndicatorHelper")->SetActive(false);
	current_crystal_to_spawn->GetTransform()->SetGlobalPosition(emitter_position);

	_current_index_crystals = (_current_index_crystals + 1) % _explosive_crystals.size();

	combat_state = CombatState::IDLE;
}

void Hachiko::Scripting::BossController::ConsumeParasytes()
{
}

void Hachiko::Scripting::BossController::ConsumeParasytesController()
{
}

void Hachiko::Scripting::BossController::FocusCamera(bool focus_on_boss)
{
	camera_focus_on_boss = focus_on_boss;
	if (camera_focus_on_boss)
	{
		level_manager->BlockInputs(true);
		player_camera->SetObjective(game_object);
	}
	else
	{
		level_manager->BlockInputs(false);
		player_camera->SetObjective(player);
	}
}

void Hachiko::Scripting::BossController::SpawnEnemy()
{
	enemy_timer += Time::DeltaTime();
	if (enemy_timer < time_between_enemies)
	{
        return;
    }

    for (EnemyController* enemy_controller : enemies) 
    {
        GameObject* enemy = enemy_controller->GetGameObject();

        if (enemy->IsActive())
        {
            continue;
        }

        enemy->SetActive(true);
        ComponentAgent* agent = enemy->GetComponent<ComponentAgent>();
        
        if (agent)
        {
            agent->AddToCrowd();
        }
            
        break;
    }

    enemy_timer = 0;
}

void Hachiko::Scripting::BossController::ResetEnemies()
{
	for (EnemyController* enemy_controller : enemies)
	{
        GameObject* enemy = enemy_controller->GetGameObject();
		ComponentAgent* agent = enemy_controller->GetGameObject()->GetComponent<ComponentAgent>();
		
        if (agent)
		{
			agent->RemoveFromCrowd();
		}
		
        if (enemy_controller)
		{
            // Maybe pack these methods into a method of EnemyController for
            // ease of extensibility.
			enemy_controller->SetIsFromBoss(true);
			enemy_controller->ResetEnemy();
			enemy_controller->ResetEnemyPosition();
		}
		
        enemy->SetActive(false);
	}
}
