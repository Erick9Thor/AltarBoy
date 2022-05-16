#include "core/hepch.h"
#include "SceneSerializer.h"

#include "core/preferences/src/ResourcesPreferences.h"

Hachiko::Scene* Hachiko::SceneSerializer::Load(const char* path)
{
    YAML::Node scene_node = YAML::LoadFile(path);

    // Validate scene header
    if (!scene_node[SCENE_NAME].IsDefined())
    {
        return nullptr;
    }
    preferences = App->preferences->GetResourcesPreference();
    const auto scene_output = new Scene();

    scene_output->Load(scene_node);

    return scene_output;
}

bool Hachiko::SceneSerializer::Save(const Scene* scene, const char* scene_path)
{
    YAML::Node scene_data;
    std::string output_path;

    scene->Save(scene_data);

    if (!scene_path)
    {
        output_path = StringUtils::Concat(
            App->preferences->GetResourcesPreference()->GetAssetsPath(Resource::Type::SCENE),
            scene->GetName(), SCENE_EXTENSION);
    }
    else
    {
        output_path = scene_path;
    }

    FileSystem::Save(scene_path, scene_data);

    return true;
}
