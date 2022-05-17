#include "core/hepch.h"

#include "SceneImporter.h"

void Hachiko::SceneImporter::Import(const char* path, YAML::Node& meta) {}

void Hachiko::SceneImporter::Save(UID id, const Resource* material) {}

Hachiko::Resource* Hachiko::SceneImporter::Load(UID id)
{
    return nullptr;
}

Hachiko::UID Hachiko::SceneImporter::CreateEmptyScene(const std::string& name)
{
    return UID();
}
