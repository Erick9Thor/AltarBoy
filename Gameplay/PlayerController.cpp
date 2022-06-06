#include "scriptingUtil/gameplaypch.h"
#include "PlayerController.h"
#include "Scenes.h"
#include "EnemyController.h"
#include "CrystalExplosion.h"
#include "PlayerCamera.h"
#include "BulletController.h"

#include <components/ComponentTransform.h>
#include <components/ComponentCamera.h>
#include <modules/ModuleSceneManager.h>

#define RAD_TO_DEG 180.0f / math::pi

Hachiko::Scripting::PlayerController::PlayerController(GameObject* game_object)
	: Script(game_object, "PlayerController")
	, _attack_indicator(nullptr)
	, _goal(nullptr)
	, _dash_duration(0.0f)
	, _dash_distance(0.0f)
	, _dash_cooldown(0.0f)
	, _max_dash_count(0)
	, _attack_duration(0.0f)
	, _rotation_duration(0.0f)
	, _combat_stats()
	, _state(PlayerState::IDLE)
	, _camera(nullptr)
	, _ui_damage(nullptr)
{
}

void Hachiko::Scripting::PlayerController::OnAwake()
{
	_dash_count = _max_dash_count;

	if (_attack_indicator)
	{
		_attack_indicator->SetActive(false);
	}
	enemies = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Enemies");
	dynamic_envi = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Crystals");

	_combat_stats = game_object->GetComponent<Stats>();
	_combat_stats->_attack_power = 2;
	_combat_stats->_attack_cd = 1;
	_combat_stats->_move_speed = 7.0f;
	_combat_stats->_max_hp = 4;
	_combat_stats->_current_hp = _combat_stats->_max_hp;

	if (!_hp_cell_1 || !_hp_cell_2 || !_hp_cell_3 || !_hp_cell_4)
	{
		HE_LOG("Error loading HP Cells UI");
		return;
	}

	hp_cells.push_back(_hp_cell_1);
	hp_cells.push_back(_hp_cell_2);
	hp_cells.push_back(_hp_cell_3);
	hp_cells.push_back(_hp_cell_4);
}

void Hachiko::Scripting::PlayerController::OnUpdate()
{
	ComponentTransform* transform = game_object->GetTransform();
	math::float3 current_position = transform->GetGlobalPosition();
	math::float3 moving_input_dir = float3::zero;

	// Attack:
	Attack(transform, current_position);

	// Handle all the input:
	HandleInput(current_position, moving_input_dir);

	// Dash:
	Dash(current_position);

	// Rotate player to the necessary direction:
	Rotate(transform, moving_input_dir);

	// Apply the position:
	transform->SetGlobalPosition(current_position);

	//CheckGoal(current_position);
}

PlayerState Hachiko::Scripting::PlayerController::GetState() const
{
	return _state;
}

math::float3 Hachiko::Scripting::PlayerController::GetRaycastPosition(
	const math::float3& current_position) const
{
	const math::Plane plane(math::float3(0.0f, current_position.y, 0.0f),
		math::float3(0.0f, 1.0f, 0.0f));

	const math::float2 mouse_position_view =
		ComponentCamera::ScreenPositionToView(Input::GetMouseNormalizedPosition());

	const math::LineSegment ray = Debug::GetRenderingCamera()->Raycast(
		mouse_position_view.x, mouse_position_view.y);

	return plane.ClosestPoint(ray);
}

void Hachiko::Scripting::PlayerController::SpawnGameObject() const
{
	static int times_hit_g = 0;

	if (!Input::IsKeyDown(Input::KeyCode::KEY_G))
	{
		return;
	}

	std::string name = "GameObject ";

	name += std::to_string(times_hit_g);

	times_hit_g++;

	GameObject* created_game_object = GameObject::Instantiate();

	created_game_object->SetName(name);
}

void Hachiko::Scripting::PlayerController::Attack(ComponentTransform* transform,
	math::float3& current_position)
{
	if (_attack_current_duration > 0.0f)
	{
		if (_attack_indicator)
		{
			_attack_indicator->SetActive(true);
		}

		_attack_current_duration -= Time::DeltaTime();
	}
	else
	{
		if (_attack_indicator)
		{
			_attack_indicator->SetActive(false);
		}
		// Set state to idle, it will be overriden if there is a movement:
		_state = PlayerState::IDLE;

	}

	if (_is_dashing || (_attack_current_cd > 0.0f) )
	{
		_attack_current_cd -= Time::DeltaTime();
		return;
	}

	if (Input::IsMouseButtonDown(Input::MouseButton::RIGHT))
	{
		// Make the player look the mouse:
		transform->LookAtTarget(GetRaycastPosition(current_position));
		_state = PlayerState::RANGED_ATTACKING;
		float3 t_pos = current_position;
		t_pos.y += 0.5;
		RangedAttack(transform, t_pos);

		_attack_current_cd = _combat_stats->_attack_cd;
	}
	else if (Input::IsMouseButtonDown(Input::MouseButton::LEFT))
	{
		_attack_current_duration = _attack_duration; // For now we'll focus on melee attacks
		transform->LookAtTarget(GetRaycastPosition(current_position));
		_state = PlayerState::MELEE_ATTACKING;
		MeleeAttack(transform, current_position);
		current_position += transform->GetFront() * 0.3f;
		_attack_current_cd = _combat_stats->_attack_cd;
	}
}

void Hachiko::Scripting::PlayerController::MeleeAttack(ComponentTransform* transform,
	const math::float3& current_position)
{
	if (enemies == nullptr && dynamic_envi == nullptr) 
	{
		return;
	}

	std::vector<GameObject*> enemy_children = enemies->children;
	std::vector<GameObject*> environment = dynamic_envi->children;

	// MELEE

	enemy_children.insert(enemy_children.end(), environment.begin(), environment.end());

	GameObject* inter = game_object->scene_owner->GetRoot();

	std::vector<GameObject*> elements_hit = {};
	math::float4x4 inv_matrix = transform->GetGlobalMatrix().Transposed();
	for (int i = 0; i < enemy_children.size(); ++i)
	{
		if (enemy_children[i]->active && _combat_stats->_attack_range >= transform->GetGlobalPosition().Distance(enemy_children[i]->GetTransform()->GetGlobalPosition()))
		{
			float3 normalized_center = transform->GetFront().Normalized();
			float3 normalized_enemy = (enemy_children[i]->GetTransform()->GetGlobalPosition() - transform->GetGlobalPosition()).Normalized();
			float dot_product = normalized_center.Dot(normalized_enemy);
			float angle_of_enemy = std::acos(dot_product) * RAD_TO_DEG;
			float attack_angle = 60.0f; // This can vary in the future deppending of weapon
			if (angle_of_enemy < attack_angle)
			{
				elements_hit.push_back(enemy_children[i]);
			}
		}
	}

	//loop in enemies hit
	for (Hachiko::GameObject* element : elements_hit)
	{
		EnemyController* enemy_controller = element->GetComponent<EnemyController>();
		CrystalExplosion* crystal_explotion = element->GetComponent<CrystalExplosion>();

		float3 relative_dir = (element->GetTransform()->GetGlobalPosition() 
			- transform->GetGlobalPosition()).Normalized();

		if (enemy_controller != nullptr)
		{
			enemy_controller->RegisterPlayerHit(_combat_stats->_attack_power, relative_dir);
		}
		else if (crystal_explotion != nullptr)
		{
			crystal_explotion->ReceiveDamage(_combat_stats->_attack_power, relative_dir);
		}
	}

	if (elements_hit.size() > 0)
	{
		_camera->GetComponent<PlayerCamera>()->Shake(0.6f, 0.2f);
	}
	
}

void Hachiko::Scripting::PlayerController::RangedAttack(ComponentTransform* transform,
	const math::float3& current_position)
{
	// Spawn bullet
	GameObject* bullet = GameObject::Instantiate(14999767472668584259, game_object->scene_owner->GetRoot());
	bullet->GetTransform()->SetGlobalPosition(current_position);
	bullet->GetComponent<BulletController>()->SetForward((GetRaycastPosition(current_position) - current_position).Normalized());
	bullet->GetComponent<BulletController>()->SetDamage(_combat_stats->_attack_power);
}

void Hachiko::Scripting::PlayerController::Dash(math::float3& current_position)
{
	// This is a rather primitive dash that should be used as a base to build 
	// a more polished one. Some related issues needed to be fixed are:
	// 1. It should have a cooldown.
	// 2. As we use the camera that has PlayerCamera script is attached to now,
	//    while the camera is lerping with a delay, the corresponding world 
	//    position of mouse position changes as the camera approaches its final
	//    position, that causes dash to seem kind of unstable.
	// 3. Instead of approaching to _dash_end linearly, dash must have some sort
	//    of an acceleration.
	// TODO: Address the issues above. 
	if (_dash_timer > 0.0f)
	{
		_dash_timer += Time::DeltaTime();
	}

	if (_dash_timer >= _dash_cooldown)
	{
		if (_dash_count < _max_dash_count)
		{
			_dash_timer = 0.0001f;
			_dash_count += 1;
		}
		else if (_dash_count == _max_dash_count)
		{
			_dash_timer = 0.0f;
		}

	}
	else
	{
		if (_dash_count == _max_dash_count)
		{
			_dash_timer = 0.0f;
		}
	}

	_dash_has_cooldown = (_dash_count <= 0);

	if (!_is_dashing)
	{
		return;
	}

	// Set state to dashing:
	_state = PlayerState::DASHING;

	_dash_progress += Time::DeltaTime() / _dash_duration;
	_dash_progress = _dash_progress > 1.0f ? 1.0f : _dash_progress;

	_is_dashing = (_dash_progress < 1.0f);

	const math::float3 _dash_end =
		_dash_start + _dash_direction * _dash_distance;

	current_position = math::float3::Lerp(_dash_start, _dash_end,
		_dash_progress);
}

void Hachiko::Scripting::PlayerController::Rotate(
	ComponentTransform* transform, const math::float3& moving_dir_input)
{
	// For now dont update if player is Attacking, later with custom animations this may change
	if (_state == PlayerState::MELEE_ATTACKING ||
		_state == PlayerState::RANGED_ATTACKING)
	{
		return;
	}

	// If input tells to turn to player
	if (!moving_dir_input.Equals(float3::zero) && _state == PlayerState::WALKING)
	{

		// Get the rotation player is going to have:
		const math::Quat target_rotation = 
			transform->SimulateLookAt(moving_dir_input);

		// If rotation is gonna be changed fire up the rotating process:
		if (!target_rotation.Equals(transform->GetGlobalRotation()))
		{
			// Reset all the variables related to rotation lerp:
			_rotation_progress = 0.0f;
			_rotation_target = target_rotation;
			_rotation_start = transform->GetGlobalRotation();
			_should_rotate = true;
		}
	}

	// Smoothly rotate to the new direction of the player in _rotation_duration
	// amount of seconds:
	if (_should_rotate)
	{
		_rotation_progress += Time::DeltaTime() / _rotation_duration;
		_rotation_progress = _rotation_progress > 1.0f
			? 1.0f
			: _rotation_progress;

		transform->SetGlobalRotation(Quat::Lerp(_rotation_start, _rotation_target, _rotation_progress));

		_should_rotate = _rotation_progress < 1.0f;

		if (!_should_rotate)
		{
			_rotation_progress = 0.0f;
			_rotation_target = math::Quat::identity;
			_rotation_start = math::Quat::identity;
			_should_rotate = false;
		}
	}
}

void Hachiko::Scripting::PlayerController::HandleInput(math::float3& current_position, math::float3& moving_input_dir)
{
	// Ignore the inputs if engine camera input is taken:
	// Do we need this?
	if (Input::IsMouseButtonPressed(Input::MouseButton::RIGHT))
	{
		return;
	}

	if (!_god_mode && _is_falling)
	{
		current_position.y -= 0.25f;

		float3 corrected_position = Navigation::GetCorrectedPosition(current_position, float3(10.0f, 10.0f, 10.0f));
		_state = PlayerState::FALLING;
		if (Distance(corrected_position, current_position) < 1.0f)
		{
			current_position = corrected_position;
			_is_falling = false;
		}

		if (current_position.y < -20.0f)
		{
			//SceneManagement::SwitchScene(Scenes::LOSE);
		}
		return;
	}

	// Dashing locks player from submitting new commands on input:
	if (_is_dashing)
	{
		if (_god_mode)
		{
			return;
		}

		// check if in navmesh
		float3 corrected_position = Navigation::GetCorrectedPosition(current_position, float3(10.0f, 10.0f, 10.0f));

		if (Distance(corrected_position, current_position) >= 1.0f)
		{
			_is_falling = true;
		}
		else
		{
			current_position = corrected_position;
		}

		return;
	}

	// Lock the player movement while attacking, this is gonna be 
	// probably changed after root motion anims are implemented:
	if (_state == PlayerState::MELEE_ATTACKING ||
		_state == PlayerState::RANGED_ATTACKING)
	{
		return;
	}

	const float velocity = _combat_stats->_move_speed * Time::DeltaTime();
	const math::float3 delta_x = math::float3::unitX * velocity;
	const math::float3 delta_y = math::float3::unitY * velocity;
	const math::float3 delta_z = math::float3::unitZ * velocity;
	
	if (Input::IsKeyPressed(Input::KeyCode::KEY_W))
	{
		current_position -= delta_z;
		moving_input_dir += math::float3::unitZ;
		_state = PlayerState::WALKING;
	}
	else if (Input::IsKeyPressed(Input::KeyCode::KEY_S))
	{
		current_position += delta_z;
		moving_input_dir -= math::float3::unitZ;
		_state = PlayerState::WALKING;
	}

	if (Input::IsKeyPressed(Input::KeyCode::KEY_D))
	{
		current_position += delta_x;
		moving_input_dir -= math::float3::unitX;
		_state = PlayerState::WALKING;
	}
	else if (Input::IsKeyPressed(Input::KeyCode::KEY_A))
	{
		current_position -= delta_x;
		moving_input_dir += math::float3::unitX;
		_state = PlayerState::WALKING;
	}

	if (_god_mode && Input::IsKeyPressed(Input::KeyCode::KEY_Q))
	{
		current_position += delta_y;
	}
	else if (_god_mode && Input::IsKeyPressed(Input::KeyCode::KEY_E))
	{
		current_position -= delta_y;
	}

	if (Input::IsKeyDown(Input::KeyCode::KEY_SPACE))
	{
		_dash_has_cooldown = (_dash_count <= 0);
		if (_dash_has_cooldown)
		{
			return;
		}
		_dash_count -= 1;
		_is_dashing = true;
		_dash_progress = 0.0f;
		_dash_start = current_position;
		_dash_timer = _dash_timer == 0.0f ? 0.0001f : _dash_timer;

		_dash_direction = game_object->GetTransform()->GetFront();
		_dash_direction.Normalize();
	}

	if (Input::IsKeyDown(Input::KeyCode::KEY_G))
	{
		//_god_mode = !_god_mode;
		RegisterHit(1);
	}
	
	if (!_god_mode && _state == PlayerState::WALKING)
	{
		current_position = Navigation::GetCorrectedPosition(current_position, float3(10.0f, 10.0f, 10.0f));
	}
}

void Hachiko::Scripting::PlayerController::RegisterHit(int enemy_attack_power)
{
	if (_god_mode)
	{
		return;
	}

	_combat_stats->ReceiveDamage(enemy_attack_power);
	UpdateHealthBar();
	_camera->GetComponent<PlayerCamera>()->Shake(0.6f, 0.2f);

	// Player is dead
	if (_combat_stats->_current_hp <= 0)
	{
		SceneManagement::SwitchScene(Scenes::LOSE);
	}
}

void Hachiko::Scripting::PlayerController::UpdateHealthBar()
{
	// Disable cells 
	for (int i = 0; i < hp_cells.size(); ++i)
	{
		if (i >= _combat_stats->_current_hp)
		{
			hp_cells[i]->GetComponent(Component::Type::IMAGE)->Disable();
		}
	}
}

void Hachiko::Scripting::PlayerController::CheckGoal(const float3& current_position)
{
	const float3 goal_position = _goal->GetTransform()->GetGlobalPosition();

	if (Distance(current_position, goal_position) < 10.0f)
	{
		//SceneManagement::SwitchScene(Scenes::WIN);
	}
}