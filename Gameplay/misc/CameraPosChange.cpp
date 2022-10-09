#include "scriptingUtil/gameplaypch.h"
#include "constants/Scenes.h"
#include "misc/CameraPosChange.h"
#include "entities/player/PlayerCamera.h"

#include <modules/ModuleSceneManager.h>

Hachiko::Scripting::CameraPosChange::CameraPosChange(GameObject* game_object)
	: Script(game_object, "CameraPosChange")
	, _objective(nullptr)
	, _do_look_ahead(false)
    , _speed(0.0f)
    , _duration(0.0f)
    , _relative_position(float3::zero)
    , _rotation(float3::zero)
	, _check_box(false)
{
}

void Hachiko::Scripting::CameraPosChange::OnAwake()
{
	_player = Scenes::GetPlayer();
	_camera = Scenes::GetMainCamera()->GetComponent<PlayerCamera>();
	global_pos = game_object->GetTransform()->GetGlobalPosition();
	local_matrix = game_object->GetTransform()->GetLocalMatrix();
	boundingbox = OBB(local_matrix.Col3(3), float3(1.f, 1.f, 1.f), local_matrix.WorldX().Normalized(), local_matrix.WorldY().Normalized(), local_matrix.WorldZ().Normalized());
}

void Hachiko::Scripting::CameraPosChange::OnStart()
{
}

void Hachiko::Scripting::CameraPosChange::OnUpdate()
{
	if (_player && _camera)
	{
		float distance = Distance(global_pos, _player->GetTransform()->GetGlobalPosition());
		if (distance < 3.0f)
		{
			_is_inside = true;
		}
		else 
		{
			if (_is_inside)
			{
				_camera->ChangeRelativePosition(_relative_position, _do_look_ahead, _speed, _duration);
				_camera->RotateCameraTo(_rotation, _speed);
				if (_objective)
				{
					_camera->SetObjective(_objective);
				}
			}
			_is_inside = false;
		}
	}
}

bool Hachiko::Scripting::CameraPosChange::InsideOBB()
{
	float3 player_pos = _player->GetTransform()->GetGlobalPosition();
	Sphere hitbox = Sphere(player_pos, 0.5f);

	return boundingbox.Intersects(hitbox);
}