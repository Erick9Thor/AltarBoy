#include "scriptingUtil/gameplaypch.h"
#include "DoorController.h"
#include "components/ComponentObstacle.h"

Hachiko::Scripting::DoorController::DoorController(GameObject* game_object)
	: Script(game_object, "DoorController")
	, _door_open(nullptr)
	, _door_closed(nullptr)
	
{}

void Hachiko::Scripting::DoorController::OnAwake()
{
	_door_open = _door_open ? _door_open : game_object->FindDescendantWithName("Door(open)");
	_door_closed = _door_closed ? _door_closed : game_object->FindDescendantWithName("Door(closed)");
}

void Hachiko::Scripting::DoorController::OnStart()
{
}

void Hachiko::Scripting::DoorController::OnUpdate()
{
	if (Input::IsKeyDown(Input::KeyCode::KEY_1))
	{
		_state = _state == State::CLOSED ? State::OPEN : State::CLOSED;
		SetDoorState(_state);
	}
}

void Hachiko::Scripting::DoorController::SetDoorState(const State state)
{
	switch (state)
	{
	case State::CLOSED:
		_door_open->SetActive(false);
		_door_closed->SetActive(true);
		_door_closed->GetComponent<ComponentObstacle>()->AddObstacle();
		break;
	case State::OPEN:
		_door_open->SetActive(true);
		_door_closed->SetActive(false);
		_door_closed->GetComponent<ComponentObstacle>()->RemoveObstacle();
		break;
	}
}