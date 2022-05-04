#include "core/hepch.h"
#include "AnimationController.h"

Hachiko::AnimationController::AnimationController() {}

Hachiko::AnimationController::~AnimationController() {
    Stop();
}

void Hachiko::AnimationController::Play(UID clip, bool loop, unsigned fade_time) {
    Instance* new_instance = new Instance;
    new_instance->clip = clip;
    new_instance->loop = loop;
    new_instance->fade_duration = fade_time;
    new_instance->next = current;

    current = new_instance;
}

void Hachiko::AnimationController::Update(unsigned elapsed) {
    if (current != nullptr)
    {
        UpdateInstance(current, elapsed);
    }
}

void Hachiko::AnimationController::Stop() {
    if (current != nullptr)
    {
        ReleaseInstance(current);
        current = nullptr;
    }
}

void Hachiko::AnimationController::UpdateInstance(Instance* instance, unsigned elapsed) {}

void Hachiko::AnimationController::ReleaseInstance(Instance* instance)
{
    do
    {
        Instance* next = instance->next;
        delete instance;
        instance = next;
    } while (instance != nullptr);
}

bool Hachiko::AnimationController::GetTransform(const std::string& channel_name, math::float3& position, Quat& rotation) const
{
    return false;
}

float Hachiko::AnimationController::Interpolate(float first, float second, float lambda)
{
    return first * (1.0f - lambda) + second * lambda;
}

float3 Hachiko::AnimationController::Interpolate(const float3& first, const float3& second, float lambda)
{
    return float3::Lerp(first, second, lambda);
}

Quat Hachiko::AnimationController::Interpolate(const Quat& first, const Quat& second, float lambda) const
{
    if (first.Dot(second) >= 0.0f)
    {
        return Quat::Lerp(first, second, lambda).Normalized();
    }
    return Quat::Lerp(first, second.Neg(), lambda).Normalized();
}