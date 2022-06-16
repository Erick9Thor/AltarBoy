#include "scriptingUtil/gameplaypch.h"
#include "DoorController.h"
#include "components/ComponentObstacle.h"

Hachiko::Scripting::DoorController::DoorController(GameObject* game_object)
	: Script(game_object, "DoorController")
	, _door_open(nullptr)
	, _door_closed(nullptr)
	, _enemies(nullptr)
{}

void Hachiko::Scripting::DoorController::OnAwake()
{
	_door_open = _door_open ? _door_open : game_object->FindDescendantWithName("Door(open)");
	_door_closed = _door_closed ? _door_closed : game_object->FindDescendantWithName("Door(closed)");
	if (!_enemies)
	{
		_state = State::OPEN;
		UpdateDoorState();
	}
}

void Hachiko::Scripting::DoorController::OnStart()
{
	
}

void Hachiko::Scripting::DoorController::OnUpdate()
{
	if (Input::IsKeyDown(Input::KeyCode::KEY_1))
	{
		_state = _state == State::CLOSED ? State::OPEN : State::CLOSED;
		UpdateDoorState();
	}

	if (_state == State::CLOSED)
	{
		if (CheckOpeningConditions())
		{
			_state = State::OPEN;
			UpdateDoorState();
		}
	}
}

bool Hachiko::Scripting::DoorController::CheckOpeningConditions() const
{
	if (_enemies->children.size() > 0)	// There are enemies remaining
	{
		return false;
	}
	else	// There are no enemies alive
	{
		return true;
	}
}

void Hachiko::Scripting::DoorController::UpdateDoorState()
{
	switch (_state)
	{
	case State::CLOSED:
		_door_open->SetActive(false);
		_door_closed->SetActive(true);
		break;
	case State::OPEN:
		_door_open->SetActive(true);
		_door_closed->SetActive(false);
		SceneManagement::Destroy(_door_closed);
		break;
	}
}