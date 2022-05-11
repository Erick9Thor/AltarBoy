#pragma once

#include <scripting/Script.h>
#include "PlayerState.h"

namespace Hachiko
{

class GameObject;

namespace Scripting
{

class PlayerController;

class PlayerAnimationManager : public Script
{
	SERIALIZATION_METHODS(false)

public:
	PlayerAnimationManager(Hachiko::GameObject* game_object);
	~PlayerAnimationManager() override = default;

	void OnAwake() override;
	void OnStart() override;
	void OnUpdate() override;

private:
	PlayerController* _player_controller;

	SERIALIZE_FIELD(PlayerState, _previous_state);
	SERIALIZE_FIELD(std::string, _state_string);
};
}
} // namespace Hachiko

