#include "scriptingUtil/gameplaypch.h"
#include "LaserController.h"
#include "constants/Scenes.h"
#include "entities/player/PlayerController.h"

//#include "components/ComponentObstacle.h"

// TODO: Delete this include:
#include <modules/ModuleSceneManager.h>

Hachiko::Scripting::LaserController::LaserController(GameObject* game_object)
	: Script(game_object, "LaserController")
	, _laser(nullptr)
	, _max_length(5.0f)
	, _max_scale(0.2f)
	, _activation_time(1.0f)
	, _damage(1.0f)
{}

void Hachiko::Scripting::LaserController::OnAwake()
{
	_terrain = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Level");
	_player = game_object->scene_owner->GetRoot()->GetFirstChildWithName("Player");
	//door_obstacle = _door->GetComponent<ComponentObstacle>();
}

void Hachiko::Scripting::LaserController::OnUpdate()
{
	if (Input::IsKeyDown(Input::KeyCode::KEY_1))
	{
		if (State::ACTIVE) ChangeState(State::INACTIVE);
		if (State::INACTIVE) ChangeState(State::ACTIVATING);
	}

	switch (_state)
	{
	case State::ACTIVE:
		AdjustLength();
		CheckPlayerCollision();
		break;
	case State::ACTIVATING:
		AdjustLength();
		_elapse_time += Time::DeltaTime();
		_scale = (_elapse_time / _activation_time) * _max_scale;
		if (_elapse_time >= _activation_time) 
		{
			ChangeState(State::ACTIVE);
		}
		break;
	case State::INACTIVE:
		break;
	}
}

void Hachiko::Scripting::LaserController::ChangeState(State new_state) 
{
	switch (new_state)
	{
	case State::ACTIVE:
		_scale = _max_scale;
		break;
	case State::ACTIVATING:
		_elapse_time = 0.0f;
		_scale = 0.0f;
		AdjustLength();

		// TODO: change tint color to 0 alpha
		_laser->ChangeColor(float4(1.0f, 1.0f, 1.0f, 1.0f), _activation_time, true); // remove this when tint is here
		_laser->SetActive(true);
		break;
	case State::INACTIVE:
		_laser->SetActive(false);
		break;
	}

	_state = new_state;
}

void Hachiko::Scripting::LaserController::AdjustLength()
{
	float3 collision_point;
	float new_length = _max_length;
	
	ComponentTransform* transform = _laser->GetComponent<ComponentTransform>();
	float3 start = transform->GetGlobalPosition();
	float3 end = start + transform->GetFront() * _max_length;


	GameObject* terrain_hit = SceneManagement::Raycast(start, end, &collision_point, _terrain);
	if (terrain_hit != nullptr)
	{
		new_length = std::min(start.Distance(collision_point), _max_length);
	}

	if (_length != new_length || _state == State::ACTIVATING)
	{
		_length = new_length;
		transform->SetLocalScale(float3(_scale, _scale, _length * 0.5f));
	}
}

void Hachiko::Scripting::LaserController::CheckPlayerCollision()
{
	float3 intersection_point;
	float3 player_pos = _player->GetComponent<ComponentTransform>()->GetGlobalPosition();
	const OBB* laser_obb = _laser->GetFirstMeshRendererOBB();

	Sphere hitbox = Sphere(player_pos, 0.5f); // player radious

	if (laser_obb->Intersects(hitbox, &intersection_point))
	{
		/*ComponentTransform* transform = _laser->GetComponent<ComponentTransform>();
		float3 point_in_line = transform->GetGlobalPosition();
		float3 line = transform->GetFront();
		float torque = line.x * (intersection_point.z - point_in_line.z) - line.y * (intersection_point.x - point_in_line.x);

		float3 knockback = transform->GetRight();
		if (torque > 0) 
		{
			knockback *= -1;
		}

		_player->GetComponent<PlayerController>()->RegisterHit(_damage, true, knockback);*/

		_player->GetComponent<PlayerController>()->RegisterHit(_damage);
	}
}

