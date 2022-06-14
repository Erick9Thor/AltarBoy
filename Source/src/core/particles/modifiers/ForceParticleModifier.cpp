#include "core/hepch.h"
#include "core/particles/modifiers/ForceParticleModifier.h"

Hachiko::ForceParticleModifier::ForceParticleModifier(const std::string& name) :
    ParticleModifier(name, false)
{
    cfg.min = -100.0f;
    cfg.max = 100.0f;
    cfg.speed = 0.1f;
    cfg.format = "%.2f";
}

void Hachiko::ForceParticleModifier::Update(std::vector<Particle>& particles)
{
    for (auto& particle : particles)
    {
        if (!particle.IsActive())
        {
            continue;
        }

        UpdatePositionOverTime(particle);
        // UpdateDirectionOverTime(particle);
        UpdateRotationOverTime(particle);
    }
}

void Hachiko::ForceParticleModifier::DrawGui()
{
    MultiTypeSelector("Rotation", rotation_delta, &cfg);
}

void Hachiko::ForceParticleModifier::Save(YAML::Node& node) const
{
    YAML::Node force_module = node[MODULE_FORCE];
    ParticleModifier::Save(force_module);
    force_module[ROTATION] = rotation_delta;
}

void Hachiko::ForceParticleModifier::Load(const YAML::Node& node)
{
    ParticleModifier::Load(node[MODULE_FORCE]);
    rotation_delta = node[MODULE_FORCE][ROTATION].IsDefined() ? node[MODULE_FORCE][ROTATION].as<ParticleSystem::VariableTypeProperty>() : rotation_delta;
}

void Hachiko::ForceParticleModifier::UpdatePositionOverTime(Particle& particle) 
{
    float3 position = particle.GetCurrentPosition();
    position += particle.GetCurrentDirection() * particle.GetCurrentSpeed();
    particle.SetCurrentPosition(position);
}

void Hachiko::ForceParticleModifier::UpdateDirectionOverTime(Particle& particle)
{
    //float3 direction = particle.GetCurrentDirection();
    //direction = float3(Random::RandomFloat(), Random::RandomFloat(), Random::RandomFloat());
    //direction.Normalize();
    //particle.SetCurrentDirection(direction);
}

void Hachiko::ForceParticleModifier::UpdateRotationOverTime(Particle& particle) 
{
    float rotation = particle.GetCurrentRotation();
    rotation += rotation_delta.GetValue(1 - (particle.GetCurrentLife() / particle.GetLife()));
    particle.SetCurrentRotation(rotation);
}
