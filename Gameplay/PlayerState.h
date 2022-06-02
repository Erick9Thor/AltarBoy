#pragma once

enum class PlayerState
{
	INVALID,
	IDLE,
	WALKING,
	MELEE_ATTACKING,
	RANGED_ATTACKING,
	DASHING,
	FALLING
};

enum class BugState
{
	INVALID,
	DEAD,
	IDLE,
	ATTACKING
};