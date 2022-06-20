#include "scriptingUtil/gameplaypch.h"

#include "core/GameObject.h"

#include "CrystalPlatform.h"

#include <resources/ResourceAnimation.h>


Hachiko::Scripting::CrystalPlatform::CrystalPlatform(GameObject* game_object)
	: Script(game_object, "CrystalPlatform")
	, _seconds_before_shaking(5.0f)
	, _seconds_shaking(4.0f)
	, _invisible_obstacle(nullptr)
	, _crystal(nullptr)
	, _crystal_platform(nullptr)
{
}

void Hachiko::Scripting::CrystalPlatform::OnAwake()
{}

void Hachiko::Scripting::CrystalPlatform::OnStart()
{
	exploding_platform = _crystal_platform->GetComponent<ComponentAnimation>();
	obstacle = _invisible_obstacle->GetComponent<ComponentObstacle>();
	stats = _crystal->GetComponent<Stats>();

	if (_invisible_obstacle && !_invisible_obstacle->IsActive())
	{
		_invisible_obstacle->SetActive(true);
	}
}

void Hachiko::Scripting::CrystalPlatform::OnUpdate()
{
	// Check if crystal it's alive
	if (!stats->IsAlive() && !is_platform_active)
	{
		_state = PlatformState::PLATFORM;
	}

	// If the platform is generated and animation it's finished
	// && exploding_platform->GetCurrentAnimation()->GetCurrentState() == ResourceAnimation::State::STOPPED) Add this when we have animations
	if (!is_shaking
		&& is_platform_active)
	{
		// Destroy the navmesh obstacle
		if (obstacle != nullptr && obstacle->IsInNavMesh())
		{
			obstacle->RemoveObstacle();
		}

		// Wait until start shaking
		if (_seconds_before_shaking <= 0)
		{
			_state = PlatformState::SHAKING;
		}
		else
		{
			_seconds_before_shaking -= Time::DeltaTime();
			return;
		}
	}

	// Is platform shaking
	if (is_shaking
		&& _seconds_shaking <= 0)
		// Add this line in case shaking is animation = && exploding_platform->GetCurrentAnimation()->GetCurrentState() == ResourceAnimation::State::STOPPED))
	{
		_state = PlatformState::IDLE;
	}
	else if (GetState() == PlatformState::SHAKING)
	{
		_seconds_shaking -= Time::DeltaTime();
		return;
	}

	UpdatePlatformStatus();
}

void Hachiko::Scripting::CrystalPlatform::ShowPlatform()
{
	is_platform_active = true;

	if (exploding_platform) 
	{
		exploding_platform->StartAnimating();
		// is_platform_active = true; // TODO: When we have animation this should be heare inside aniamtion
	}
}

void Hachiko::Scripting::CrystalPlatform::RegenerateCrystal()
{
	if (exploding_platform && stats) 
	{
		exploding_platform->SendTrigger("isRegenereted");
		stats->_current_hp = 10;
		is_platform_active = false;
		is_shaking = false;
	}
}

void Hachiko::Scripting::CrystalPlatform::UpdatePlatformStatus()
{
	PlatformState platform_state = GetState();
	bool state_changed = platform_state != _previous_state;

	if (!state_changed)
	{
		return;
	}

	_previous_state = platform_state;

	switch (platform_state)
	{
	case PlatformState::INVALID:
		return;
	case PlatformState::IDLE:
		RegenerateCrystal();
		break;
	case PlatformState::PLATFORM:
		ShowPlatform();
		break;
	case PlatformState::SHAKING:
		Shaking();
		break;
	default:
		break;
	}
}

void Hachiko::Scripting::CrystalPlatform::Shaking()
{
	is_shaking = true;

	float3 shake_offset = float3::zero;

	shake_offset = Shake();

	_crystal_platform->GetComponent<ComponentTransform>()->SetGlobalPosition(
		game_object->GetComponent<ComponentTransform>()->GetGlobalPosition()
		+ shake_offset);
}

void Hachiko::Scripting::CrystalPlatform::Shake(float time, float intensity)
{
	shake_time = time;
	shake_elapsed = 0.0f;
	shake_intensity = intensity;
	shake_magnitude = 1.0f;
}

float3 Hachiko::Scripting::CrystalPlatform::Shake()
{
	if (shake_elapsed < shake_time)
	{
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float x = (r - 0.5f) * shake_magnitude * shake_intensity;
		r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float z = (r - 0.5f) * shake_magnitude * shake_intensity;

		shake_elapsed += Time::DeltaTime();
		shake_magnitude = (1 - (shake_elapsed / shake_time)) * (1 - (shake_elapsed / shake_time));
		return float3(x, 0, z);
	}
	else
	{
		shake_time = 0.0f;
		shake_elapsed = 0.0f;
		return float3::zero;
	}
}
