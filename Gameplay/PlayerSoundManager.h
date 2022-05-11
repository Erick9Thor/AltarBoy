#pragma once
#include <scripting/Script.h>


namespace Hachiko
{

class GameObject;

namespace Scripting
{

class PlayerController;

class PlayerSoundManager : public Script
{

	SERIALIZATION_METHODS(false)

public:
	PlayerSoundManager(Hachiko::GameObject* game_object);
	~PlayerSoundManager() override = default;

	void OnAwake() override;
	void OnUpdate() override;

private:
	PlayerController* _player_controller;

	SERIALIZE_FIELD(float, _step_timer);
	SERIALIZE_FIELD(float, _step_frequency);
};
} // namespace Scripting
} // namespace Hachiko


