#include "scriptingUtil/gameplaypch.h"
#include "PlayerController.h"
#include "Scenes.h"
#include "Stats.h"
#include "EnemyController.h"
#include "PlayerCamera.h"
#include <components/ComponentTransform.h>
#include <components/ComponentCamera.h>
#include <modules/ModuleSceneManager.h>
#include <core/GameObject.h>

#define RAD_TO_DEG 180.0f / math::pi

Hachiko::Scripting::PlayerController::PlayerController(GameObject* game_object)
	: Script(game_object, "PlayerController")
	, _movement_speed(0.0f)
	, _dash_indicator(nullptr)
	, _dash_duration(0.0f)
	, _dash_distance(0.0f)
	, _dash_cooldown(0.0f)
	, _max_dash_count(0)
	, _attack_radius(0.0f)
	, _attack_cooldown(0.33f)
	, _attack_duration(0.0f)
	, _rotation_duration(0.0f)
	, _raycast_min_range(0.001)
	, _raycast_max_range(15.f)
	, _stats(5, 2, 10, 10)
	, _state(PlayerState::IDLE)
	, _camera(nullptr)
{
}

void Hachiko::Scripting::PlayerController::OnAwake()
{
	_dash_count = _max_dash_count;
}

void Hachiko::Scripting::PlayerController::OnUpdate()
{
	ComponentTransform* transform = game_object->GetTransform();
	math::float3 current_position = transform->GetGlobalPosition();
	math::float3 moving_input_dir = float3::zero;

	if (_stats._current_hp <= 0)
	{
		//SceneManagement::SwitchScene(Scenes::LOSE);
	}

	// Attack:
	Attack(transform, current_position);

	// Handle all the input:
	HandleInput(current_position, moving_input_dir);

	// Dash:
	Dash(current_position);

	// Rotate player to the necessary direction:
	Rotate(transform, moving_input_dir);

	// Move the dash indicator:
	//MoveDashIndicator(current_position);

	// Apply the position:
	transform->SetGlobalPosition(current_position);

	// Instantiate GameObject in current scene test:
	//SpawnGameObject();

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

void Hachiko::Scripting::PlayerController::MoveDashIndicator(
	const math::float3& current_position) const
{
	const math::float3 mouse_world_position =
		GetRaycastPosition(current_position);

	math::float3 direction = mouse_world_position - current_position;
	direction.Normalize();

	//const math::float2 mouse_direction = GetMouseDirectionRelativeToCenter();

	//_dash_indicator->GetTransform()->SetGlobalPosition(current_position
	//	+ float3(mouse_direction.x, 0.0f, mouse_direction.y));

	//_dash_indicator->GetTransform()->SetGlobalRotationEuler(
	//	float3(90.0f, 0.0f, 0.0f));
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
		_attack_current_duration -= Time::DeltaTime();
	}
	else
	{
		// Set state to idle, it will be overriden if there is a movement:
		_state = PlayerState::IDLE;

	}

	if (_is_dashing || (_attack_current_cd > 0.0f) )
	{
		_attack_current_cd -= Time::DeltaTime();
		return;
	}

	if (Input::IsMouseButtonPressed(Input::MouseButton::RIGHT))
	{
		// Make the player look the mouse:
		transform->LookAtTarget(GetRaycastPosition(current_position));
		_state = PlayerState::RANGED_ATTACKING;
		float3 t_pos = current_position;
		t_pos.y += 0.5;
		RangedAttack(transform, t_pos);

		_attack_current_cd = _attack_cooldown;
	}
	else if (Input::IsMouseButtonPressed(Input::MouseButton::LEFT))
	{
		_attack_current_duration = _attack_duration; // For now we'll focus on melee attacks
		transform->LookAtTarget(GetRaycastPosition(current_position));
		_state = PlayerState::MELEE_ATTACKING;
		MeleeAttack(transform, current_position);
		current_position += transform->GetFront() * 0.3f;
		_attack_current_cd = _attack_cooldown;
	}
}

void Hachiko::Scripting::PlayerController::MeleeAttack(ComponentTransform* transform,
	const math::float3& current_position)
{
	// MELEE
	std::vector<GameObject*> enemies = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Enemies")->children;
	std::vector<GameObject*> enemies_hit = {};
	//EnemyControler* enemy_ctrl = _player->GetComponent<PlayerController>();
	math::float4x4 inv_matrix = transform->GetGlobalMatrix().Transposed();
	for (int i = 0; i < enemies.size(); ++i)
	{
		if (enemies[i]->active && _attack_radius >= transform->GetGlobalPosition().Distance(enemies[i]->GetTransform()->GetGlobalPosition()))
		{
			float3 normalized_center = transform->GetFront().Normalized();
			float3 normalized_enemy = (enemies[i]->GetTransform()->GetGlobalPosition() - transform->GetGlobalPosition()).Normalized();
			float dot_product = normalized_center.Dot(normalized_enemy);
			float angle_of_enemy = std::acos(dot_product) * RAD_TO_DEG;
			float attack_angle = 60.0f; // This can vary in the future deppending of weapon
			if (angle_of_enemy < attack_angle)
			{
				enemies_hit.push_back(enemies[i]);
			}
		}
	}

	//loop in enemies hit
	for (Hachiko::GameObject* enemy : enemies_hit)
	{
		float3 relative_dir = enemy->GetTransform()->GetGlobalPosition() - transform->GetGlobalPosition();
		enemy->GetComponent<EnemyController>()->ReceiveDamage(_stats._attack_power, relative_dir.Normalized());
	}
	if (enemies_hit.size() > 0)
	{
		_camera->GetComponent<PlayerCamera>()->Shake(0.6f, 0.2f);
	}
	
}

void Hachiko::Scripting::PlayerController::RangedAttack(ComponentTransform* transform,
	const math::float3& current_position)
{
	const float3 forward = transform->GetFront().Normalized();
	GameObject* hit_game_object = SceneManagement::Raycast(current_position + forward * _raycast_min_range,
		forward * _raycast_max_range + current_position);

	if (hit_game_object && hit_game_object->active)
	{
		EnemyController* enemy = hit_game_object->parent->GetComponent<EnemyController>();
		if (!enemy)	return;
		float3 relative_dir = hit_game_object->GetTransform()->GetGlobalPosition() - transform->GetGlobalPosition();
		enemy->ReceiveDamage(_stats._attack_power, relative_dir.Normalized());
		_camera->GetComponent<PlayerCamera>()->Shake(0.6f, 0.3f);
	}
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

	if (!_is_god_mode && _is_falling)
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
		if (_is_god_mode)
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

	const float velocity = _movement_speed * Time::DeltaTime();
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

	if (_is_god_mode && Input::IsKeyPressed(Input::KeyCode::KEY_Q))
	{
		current_position += delta_y;
	}
	else if (_is_god_mode && Input::IsKeyPressed(Input::KeyCode::KEY_E))
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
		
		//const math::float2 mouse_direction = GetMouseDirectionRelativeToCenter();

		_dash_direction = game_object->GetTransform()->GetFront();
		_dash_direction.Normalize();
	}

	if (Input::IsKeyDown(Input::KeyCode::KEY_G))
	{
		_is_god_mode = !_is_god_mode;
		_stats._god_mode = _is_god_mode;
	}
	//float current_y = current_position.y;

	//Navigation::CorrectPosition(current_position, game_object->GetTransform()->GetGlobalScale());
	//current_position.y = current_position.y < current_y ? current_y : current_position.y;
	if (!_is_god_mode && _state == PlayerState::WALKING)
	{
		current_position = Navigation::GetCorrectedPosition(current_position, float3(10.0f, 10.0f, 10.0f));
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