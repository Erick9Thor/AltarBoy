#include "scriptingUtil/gameplaypch.h"
#include "BloomAnimator.h"

Hachiko::Scripting::BloomAnimator::BloomAnimator(GameObject* game_object)
	: Script(game_object, "BloomAnimator")
	, _target_mesh_renderer(nullptr)
	, _is_automatic(true)
	, _automatic_lerp_duration(1.0f)
	, _target_intensity(1.0f)
	, _used_lerp_duration(0)
	, _lerp_progress(0)
	, _should_return_to_automatic_mode(false)

{
}

void Hachiko::Scripting::BloomAnimator::OnAwake()
{

}

void Hachiko::Scripting::BloomAnimator::OnUpdate()
{
}

void Hachiko::Scripting::BloomAnimator::AnimateBloomManually(
	const float target_intensity, 
	const float duration,
	const bool should_return_to_automatic_mode)
{
	SetShouldAnimate(true);

	//_initial_intensity = 

}

void Hachiko::Scripting::BloomAnimator::SetShouldAnimate(bool should_animate)
{
	_should_animate = should_animate;
}
