#pragma once
#include "scripting/Script.h"

namespace Hachiko
{
namespace Scripting
{
class Experiment : public Script
{
	SERIALIZATION_METHODS(false)
public:
	Experiment(GameObject* game_object);
	~Experiment() override = default;

	void OnStart() override;
	void OnUpdate() override;
};
} // namespace Scripting
} // namespace Hachiko