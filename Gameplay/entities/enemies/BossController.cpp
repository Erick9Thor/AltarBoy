#include "scriptingUtil/gameplaypch.h"

#include "entities/enemies/BossController.h"
#include "entities/Stats.h"
#include "constants/Scenes.h"

Hachiko::Scripting::BossController::BossController(GameObject* game_object)
    : Script(game_object, "BossController")
    , state_value(0)
    , hp_bar_go(nullptr)
    , crystal_target_go(nullptr)
    , cocoon_placeholder_go(nullptr)
    , gauntlet_go(nullptr)
    , start_encounter_range(0.0f)
    , _jump_start_delay(0.0f)
	, _jump_ascending_duration(0.0f)
	, _jump_post_ascending_delay(0.0f)
	, _jump_on_highest_point_duration(0.0f)
	, _jump_post_on_highest_point_delay(0.0f)
	, _jump_descending_duration(0.0f)
	, _jump_post_descending_delay(0.0f)
	, _jump_getting_up_duration(0.0f)
	, _jump_skill_delay(0.0f)
    , _jump_skill_duration(0.0f)
	, _jump_post_on_land_aoe_duration(0.0f)
    , _jump_height(0.0f)
	, _jump_offset(0.0f)
	, _jumping_state(JumpingState::NOT_TRIGGERED)
    , _jumping_timer(0.0f)
    , _jump_pattern_index(0)
	, _boss_state_text_ui(nullptr)
	, attack_current_cd(0.0f)
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
	// Hp bar starts hidden until encounter starts
	SetHpBarActive(false);
	if (cocoon_placeholder_go)
	{
		cocoon_placeholder_go->SetActive(false);
	}
	gauntlet = gauntlet_go->GetComponent<GauntletManager>();
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
	case CombatState::JUMPING:
		ExecuteJumpingState();
		break;
	}
}

void Hachiko::Scripting::BossController::CombatTransitionController()
{
	// TODO: Actually trigger jumping by the new state machine for combat.
	if (Input::IsKeyDown(Input::KeyCode::KEY_J))
	{
		combat_state = CombatState::JUMPING;
		prev_combat_state = CombatState::CHASING;
	}

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
	case CombatState::JUMPING:
		break;
	}

	prev_combat_state = combat_state;
}

float Hachiko::Scripting::BossController::GetPlayerDistance()
{
	const float3& player_position = player->GetTransform()->GetGlobalPosition();

	// If player is very close change to attack mode
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
	// If there is any start sequence manage it here and hold state until it
	// ends. For now it goes to next state instantly:
	state = BossState::COMBAT_FORM;
}

void Hachiko::Scripting::BossController::ResetCombatState()
{
	// Reset combat related variables to prevent bugs:
	combat_state = CombatState::IDLE;
	prev_combat_state = CombatState::JUMPING;
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
	ChangeStateText("Chasing player.");
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

	float3 corrected_position = Navigation::GetCorrectedPosition(
		moving_position, 
		math::float3(10.0f));
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
	ChangeStateText("Melee attacking.");
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

	combat_state = CombatState::IDLE;
}

void Hachiko::Scripting::BossController::SpawnCrystals()
{
	ChangeStateText("Spawning crystals.");
}

void Hachiko::Scripting::BossController::SpawnCrystalsController()
{
}

void Hachiko::Scripting::BossController::ConsumeParasytes()
{
	ChangeStateText("Consuming parasites.");
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

void Hachiko::Scripting::BossController::TriggerJumpingState()
{
	combat_state = CombatState::JUMPING;

	ResetJumpingState();
}

///////////////////////////////////////////////////////////////////////////////
/// Jumping State Related Methods:

void Hachiko::Scripting::BossController::ResetJumpingState()
{
	_jumping_state = JumpingState::NOT_TRIGGERED;
	_jumping_timer = 0.0f;
}

void Hachiko::Scripting::BossController::ExecuteJumpingState()
{
	
	if (_jumping_state == JumpingState::NOT_TRIGGERED)
	{
		_jumping_state = JumpingState::WAITING_TO_JUMP;
		_jumping_timer = 0.0f;

		ChangeStateText("Waiting to jump.");

		// TODO: Trigger getting ready for jump animation here.

		// Disable the agent component, gets enabled back when boss lands back:
		agent->RemoveFromCrowd();
		agent->Disable();
	}

	_jumping_timer += Time::DeltaTime();

	switch (_jumping_state)
	{
	case JumpingState::WAITING_TO_JUMP:
	{

		if (_jumping_timer < _jump_start_delay)
		{
			// Boss is waiting for jump here:
			break;
		}

		// Boss starts jumping here:
		_jumping_state = JumpingState::ASCENDING;
		_jumping_timer = 0.0f;

		// Set the start & end positions for the jump:
		_jump_start_position = game_object->GetTransform()->GetGlobalPosition();
		_jump_end_position = player->GetTransform()->GetGlobalPosition();
		
		// Apply offset to the end position, if the offset is zero, the end
		// position of the jump will be basically equal to the player position:
		const float3 jump_direction(
			(_jump_start_position - _jump_end_position).Normalized());
		_jump_end_position = _jump_end_position + jump_direction * _jump_offset;
		_jump_end_position.y = _jump_start_position.y;

		ChangeStateText("Ascending.");

		// TODO: Trigger Jumping animation here.
	}
	break;

	case JumpingState::ASCENDING:
	{
		if (_jumping_timer < _jump_ascending_duration)
		{
			// Boss is ascending here:
			UpdateAscendingPosition();
			break;
		}

		// Boss starts waiting on the highest point of jump here:
		_jumping_state = JumpingState::POST_ASCENDING_DELAY;
		_jumping_timer = 0.0f;	


		ChangeStateText("Waiting for the delay before highest point.");
	}
	break;

	case JumpingState::POST_ASCENDING_DELAY:
	{
		if (_jumping_timer < _jump_post_ascending_delay)
		{
			// Boss waiting on the highest point here:
			break;
		}

		// Boss starts playing the highest point animation here:
		_jumping_state = JumpingState::ON_HIGHEST_POINT;
		_jumping_timer = 0.0f;

        // TODO: Trigger highest point animation here.

		ChangeStateText("On the highest point of the jump.");
	}
	break;

	case JumpingState::ON_HIGHEST_POINT:
	{
		if (_jumping_timer < _jump_on_highest_point_duration)
		{
			// Boss is on the highest point here:
			break;
		}

		// Boss starts waiting after the highest point animation here:
		_jumping_state = JumpingState::POST_ON_HIGHEST_POINT;
		_jumping_timer = 0.0f;


		ChangeStateText("Waiting before descending.");
	}
	break;

	case JumpingState::POST_ON_HIGHEST_POINT:
	{
		if (_jumping_timer < _jump_post_on_highest_point_delay)
		{
			// Boss is waiting before descending here:
			break;
		}

		// Boss starts descending animation here:
		_jumping_state = JumpingState::DESCENDING;
		_jumping_timer = 0.0f;


		ChangeStateText("Descending.");
	}
	break;


	case JumpingState::DESCENDING:
	{
		UpdateDescendingPosition();

	    if (_jumping_timer < _jump_descending_duration)
		{
			// Boss is only descending here, the update continues:
			break;
		}

		// Boss lands to the ground here:
		_jumping_state = JumpingState::POST_DESCENDING_DELAY;
		_jumping_timer = 0.0f;

		ChangeStateText("Landed & waiting.");
	}
	break;

	case JumpingState::POST_DESCENDING_DELAY:
	{
		if (_jumping_timer < _jump_post_descending_delay)
		{
			// Boss is waiting to get up here:
			break;
		}

		// Boss starts getting up here:
		_jumping_state = JumpingState::GETTING_UP;
		_jumping_timer = 0.0f;

		// TODO: Trigger boss getting up from ground animation here.

		ChangeStateText("Getting up.");
	}
	break;

	case JumpingState::GETTING_UP:
	{
		if (_jumping_timer < _jump_getting_up_duration)
		{
			// Boss is getting up here:
			break;
		}

		// Boss starts waiting for the skill here:
		_jumping_state = JumpingState::WAITING_FOR_SKILL;
		_jumping_timer = 0.0f;

		// Reset jump start and end positions to ensure correct & clean state:
		_jump_end_position = float3::zero;
		_jump_start_position = float3::zero;

		// Enable the agent component, it was disabled when the jumping was
		// started:
		agent->Enable();
		agent->AddToCrowd();

		ChangeStateText("Waiting to cast skill.");

		// TODO: Trigger Getting ready for skill animation here.
	}
	break;

	
	case JumpingState::WAITING_FOR_SKILL:
	
		if (_jumping_timer < _jump_skill_delay)
		{
			// Boss is waiting to cast skill here:
			break;
		}

		// Boss starts casting skill here:
		_jumping_state = JumpingState::CASTING_SKILL;
		_jumping_timer = 0.0f;

		// TODO: Trigger skill casting animation here.

		ChangeStateText("Casting Skill.");
		
	break;

	case JumpingState::CASTING_SKILL:

		if (_jumping_timer < _jump_skill_duration)
		{
			// Boss is casting skill here:
			break;
		}

		// Boss deals the aoe damage here:
		_jumping_state = JumpingState::POST_CAST_SKILL;
		_jumping_timer = 0.0f;

		// TODO: Deal skill damage here.
		// TODO: Trigger post skill stuff here.

		ChangeStateText("Casted Skill.");
		
	break;

	case JumpingState::POST_CAST_SKILL:

		if (_jumping_timer < _jump_post_on_land_aoe_duration)
		{
			// Boss is doing whatever it's supposed to do after it casts aoe:
			break;
		}

		// Boss finishes jump&aoe business here:
		_jumping_state = JumpingState::NOT_TRIGGERED;
		_jumping_timer = 0.0f;

		// Turn back to chasing state:
		Chase();
	break;
	}
}

void Hachiko::Scripting::BossController::InterpolatePositionWhileJumping(
	const float position_step,
	const float height_step) const
{
	float3 position = math::Lerp(
		_jump_start_position, 
		_jump_end_position, 
		position_step);

	position.y = math::Lerp(
		_jump_start_position.y, 
		_jump_height, 
		height_step);

	game_object->GetTransform()->SetGlobalPosition(position);
}

inline float CalculateAscendingHeightStep(const float position_step)
{
	return 1.0f - powf(position_step - 1.0f, 2.0f);
}

void Hachiko::Scripting::BossController::UpdateAscendingPosition() const
{
	// TODO: Take zero into account.

	// Get the step i.e percentage of ascending for x & z position:
	const float position_step = std::min(
	    1.0f, 
		_jumping_timer / _jump_ascending_duration);
	// Get the step of y position. We want it to be a parabolic jump, which
	// looks like a gravity is applied:
	const float height_step = CalculateAscendingHeightStep(position_step);

	float3 position = math::Lerp(
		_jump_start_position,
		(_jump_start_position + _jump_end_position) * 0.5f,
		position_step);

	position.y = math::Lerp(
		_jump_start_position.y,
		_jump_height,
		height_step);

	game_object->GetTransform()->SetGlobalPosition(position);
}

inline float CalculateDescendingHeightStep(const float position_step)
{
	return 1.0f - (position_step * position_step);
}

void Hachiko::Scripting::BossController::UpdateDescendingPosition() const
{
	// TODO: Take zero into account.

	// Get the step i.e percentage of descending for x & z position:
	const float position_step = std::min(
		1.0f, 
		_jumping_timer / _jump_descending_duration);
	// Get the step of y position. We want it to be a parabolic jump, which
	// looks like a gravity is applied:
	const float height_step = CalculateDescendingHeightStep(position_step);

	float3 position = math::Lerp(
		(_jump_start_position + _jump_end_position) * 0.5f,
		_jump_end_position,
		position_step);

	position.y = math::Lerp(
		_jump_start_position.y,
		_jump_height,
		height_step);

	game_object->GetTransform()->SetGlobalPosition(position);

	//InterpolatePositionWhileJumping(position_step, height_step);
}

///
///////////////////////////////////////////////////////////////////////////////

void Hachiko::Scripting::BossController::ChangeStateText(
	const char* state_string) const
{
	if (!_boss_state_text_ui)
	{
		return;
	}

	_boss_state_text_ui->SetText(state_string);
}
