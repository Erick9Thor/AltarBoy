#include "core/hepch.h"
#include "AnimationParticleModifier.h"
#include "resources/ResourceTexture.h"
#include "modules/ModuleResources.h"

Hachiko::AnimationParticleModifier::AnimationParticleModifier(const std::string& name) 
    : ParticleModifier(name, false) 
{
    cfg.min = 0.0f;
    cfg.max = 10.0f;
    cfg.speed = 0.05f;
    cfg.format = "%.2f";
}

void Hachiko::AnimationParticleModifier::Update(std::vector<Particle>& particles)
{
    if (!IsActive())
    {
        return;
    }

    time += EngineTimer::delta_time;
    if (time <= animation_speed)
    {
        return;
    }

    time = 0.0f;
    for (auto& particle : particles)
    {
        if (!particle.IsActive())
        {
            continue;
        }

        UpdateAnimation(particle);
    }
}

void Hachiko::AnimationParticleModifier::DrawGui()
{
    Widgets::Checkbox("Fit to lifetime (WIP)", &fit_to_lifetime);
    cfg.enabled = !fit_to_lifetime;
    DragFloat("Animation speed", animation_speed, &cfg);
    cfg.enabled = true;
    DragFloat("Blend factor", blend_factor, &cfg);
}

void Hachiko::AnimationParticleModifier::Save(YAML::Node& node) const 
{
    YAML::Node animation_modifier = node[MODULE_ANIMATION];
    ParticleModifier::Save(animation_modifier);
    animation_modifier[ANIMATION_SPEED] = animation_speed;
    animation_modifier[BLEND_FACTOR] = blend_factor;
}

void Hachiko::AnimationParticleModifier::Load(const YAML::Node& node) 
{
    ParticleModifier::Load(node[MODULE_ANIMATION]);
    animation_speed = node[MODULE_ANIMATION][ANIMATION_SPEED].IsDefined() ? node[MODULE_ANIMATION][ANIMATION_SPEED].as<float>() : animation_speed;
    blend_factor = node[MODULE_ANIMATION][BLEND_FACTOR].IsDefined() ? node[MODULE_ANIMATION][BLEND_FACTOR].as<float>() : blend_factor;
}

void Hachiko::AnimationParticleModifier::UpdateAnimation(Particle& particle) 
{
    if (!particle.HasTexture())
    {
        return;
    }

    particle.SetAnimationBlend(blend_factor);
    unsigned animation_frame = particle.GetCurrentAnimationFrame();
    float2 animation_idx = particle.GetAnimationIndex();
    
    if (animation_frame >= particle.GetTextureTotalTiles())
    {
        particle.SetCurrentAnimationFrame(1);
        particle.SetAnimationIndex(float2::zero);
        return;
    }

    if (animation_idx.x < particle.GetTextureTiles().x - 1)
    {
        animation_idx.x += 1.0f;
        particle.SetCurrentAnimationFrame(++animation_frame);
        particle.SetAnimationIndex(animation_idx);
        return;
    }

    if (particle.GetAnimationIndex().y < particle.GetTextureTiles().y - 1)
    {
        animation_idx.x = 0.0f;
        animation_idx.y += 1.0f;
        particle.SetCurrentAnimationFrame(++animation_frame);
        particle.SetAnimationIndex(animation_idx);
        return;
    }

    particle.SetCurrentAnimationFrame(1);
    particle.SetAnimationIndex(float2::zero);
}