#pragma once

#include <scripting/Script.h>

namespace Hachiko
{
class GameObject;
namespace Scripting
{
class PlayerCamera;
class CameraShake
{

public:
	CameraShake();
	~CameraShake() override = default;

	void Shake(float3* position);

private:
	float magnitude;
};
} // namespace Scripting
} // namespace Hachiko