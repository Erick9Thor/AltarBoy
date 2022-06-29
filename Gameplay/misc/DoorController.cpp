#include "scriptingUtil/gameplaypch.h"
#include "DoorController.h"
#include "components/ComponentObstacle.h"

Hachiko::Scripting::DoorController::DoorController(GameObject* game_object)
	: Script(game_object, "DoorController")
	, _door(nullptr)
{}

void Hachiko::Scripting::DoorController::OnAwake()
{

}

void Hachiko::Scripting::DoorController::OnUpdate()
{
	if (Input::IsKeyDown(Input::KeyCode::KEY_1))
	{
		_state = _state == State::CLOSED ? State::OPEN : State::CLOSED;
		UpdateDoorState();
	}
}

void Hachiko::Scripting::DoorController::Open()
{
	_state = State::OPEN;
	UpdateDoorState();
}

void Hachiko::Scripting::DoorController::Close()
{
	_state = State::CLOSED;
	UpdateDoorState();
}

void Hachiko::Scripting::DoorController::UpdateDoorState()
{
	switch (_state)
	{
	case State::CLOSED:
		_door->SetActive(true);
		_door->GetComponent<ComponentObstacle>()->AddObstacle();
		break;
	case State::OPEN:
		_door->SetActive(false);
		_door->GetComponent<ComponentObstacle>()->RemoveObstacle();
		break;
	}
}