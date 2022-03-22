#include "core/hepch.h"
#include "ModuleSceneManager.h"

#include "core/preferences/src/ResourcesPreferences.h"

bool Hachiko::ModuleSceneManager::Init()
{
    preferences = App->preferences->GetResourcesPreference();
    std::string scene_path = StringUtils::Concat(preferences->GetAssetsPath(Resource::Type::SCENE), preferences->GetSceneName());
    if (std::filesystem::exists(scene_path))
    {
        LoadScene(scene_path.c_str());
    }
    else
    {
        CreateEmptyScene();
    }
    return true;
}

UpdateStatus Hachiko::ModuleSceneManager::Update(const float delta)
{
    main_scene->Update();
    return UpdateStatus::UPDATE_CONTINUE;
}

bool Hachiko::ModuleSceneManager::CleanUp()
{
    SaveScene();
    RELEASE(main_scene);
    return true;
}

void Hachiko::ModuleSceneManager::CreateEmptyScene()
{
    delete main_scene;
    main_scene = new Scene();
}

void Hachiko::ModuleSceneManager::LoadScene(const char* file_path)
{
    delete main_scene;
    main_scene = serializer.Load(file_path);
}

void Hachiko::ModuleSceneManager::SaveScene()
{
    serializer.Save(main_scene);
}

void Hachiko::ModuleSceneManager::SaveScene(const char* path)
{
    serializer.Save(main_scene); // TODO: Take into account temporal scenes
}
