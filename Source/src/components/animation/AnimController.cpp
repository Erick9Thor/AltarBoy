#include "core/hepch.h"
#include "AnimController.h"

Hachiko::AnimController::AnimController() {}

Hachiko::AnimController::~AnimController() {}

void Hachiko::AnimController::Play() {}

void Hachiko::AnimController::Stop() {}

void Hachiko::AnimController::Update() {}

bool Hachiko::AnimController::GetTransform(const std::string& channel_name, math::float3& position, Quat& rotation) const
{
    return false;
}

float3 Hachiko::AnimController::Interpolate(const float3& first, const float3& second, float lambda)
{
    return first * (1.0f - lambda) + second * lambda;
}

Quat Hachiko::AnimController::Interpolate(const Quat& first, const Quat& second, float lambda) const
{
    Quat result;
    float dot = first.Dot(second);

    if (dot >= 0.0f) // Interpolate through the shortest path
    {
        result.x = first.x * (1.0f - lambda) + second.x * lambda;
        result.y = first.y * (1.0f - lambda) + second.y * lambda;
        result.z = first.z * (1.0f - lambda) + second.z * lambda;
        result.w = first.w * (1.0f - lambda) + second.w * lambda;
    }
    else
    {
        result.x = first.x * (1.0f - lambda) - second.x * lambda;
        result.y = first.y * (1.0f - lambda) - second.y * lambda;
        result.z = first.z * (1.0f - lambda) - second.z * lambda;
        result.w = first.w * (1.0f - lambda) - second.w * lambda;
    }

    result.Normalize();

    return result;
}