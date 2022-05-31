#include "core/hepch.h"
#include "core/particles/modules/SizeParticleModule.h"

Hachiko::SizeParticleModule::SizeParticleModule(const std::string& name):
    ParticleModule(name, false)
{
}

void Hachiko::SizeParticleModule::Update(ParticleSystem::Particle& particle)
{
}

void Hachiko::SizeParticleModule::DrawGui()
{
    ImGui::TextUnformatted("Size over lifetime content");
}