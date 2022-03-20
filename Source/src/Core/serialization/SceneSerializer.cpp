#include "core/hepch.h"
#include "SceneSerializer.h"

#include "Core/preferences/src/ResourcesPreferences.h"

Hachiko::Scene* Hachiko::SceneSerializer::Load(const char* path)
{
    YAML::Node scene_node = YAML::LoadFile(path);

    // Validate scene header
    if (!scene_node[SCENE_NAME].IsDefined())
    {
        return nullptr;
    }
    preferences = dynamic_cast<ResourcesPreferences*>(App->preferences->GetPreference(Preferences::Type::RESOURCES));
    const auto scene_output = new Scene();
    

    if (!scene_node[CHILD_NODE].IsDefined())
    {
        return scene_output;
    }

    scene_output->Load(scene_node);

    return scene_output;
}

bool Hachiko::SceneSerializer::Save(const Scene* scene)
{
    YAML::Node scene_data;
    scene->Save(scene_data);
    ResourcesPreferences* resources_preferences = static_cast<ResourcesPreferences*>(App->preferences->GetPreference(Preferences::Type::RESOURCES));
    std::ofstream fout(StringUtils::Concat(resources_preferences->GetAssetsPath(Resource::Type::SCENE), scene->GetName(), SCENE_EXTENSION));
    fout << scene_data;
    return true;
}

bool Hachiko::SceneSerializer::CheckIfImported(const char* path) const
{
    std::filesystem::path model_name(path);
    model_name = model_name.filename().replace_extension();
    std::string fullPath = preferences->GetLibraryPath(Resource::Type::MODEL);
    fullPath.append(model_name.string());

    return std::filesystem::exists(fullPath);
}
