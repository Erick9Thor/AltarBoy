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

#ifdef PLAY_BUILD
    App->camera->ReturnPlayerCamera(); // PLAY_BUILD UNCOMMENT
    main_scene->Start();
#endif

    return true;
}

void Hachiko::ModuleSceneManager::AttemptScenePause()
{
    if (!GameTimer::paused)
    {
        Event game_state(Event::Type::GAME_STATE);
        game_state.SetEventData<GameStateEventPayload>(GameStateEventPayload::State::PAUSED);
        App->event->Publish(game_state);

        GameTimer::Pause();
    }
}

void Hachiko::ModuleSceneManager::AttemptScenePlay()
{
    if (!GameTimer::running)
    {
        Event game_state(Event::Type::GAME_STATE);
        game_state.SetEventData<GameStateEventPayload>(GameStateEventPayload::State::STARTED);
        App->event->Publish(game_state);

        SaveScene("tmp_scene.scene");       
        
        GameTimer::Start();
    }
    else if (GameTimer::paused)
    {
        Event game_state(Event::Type::GAME_STATE);
        game_state.SetEventData<GameStateEventPayload>(GameStateEventPayload::State::RESUMED);
        App->event->Publish(game_state);
        
        GameTimer::Resume();
    }
}

void Hachiko::ModuleSceneManager::AttemptSceneStop()
{
    if (GameTimer::running)
    {
        Event game_state(Event::Type::GAME_STATE);
        game_state.SetEventData<GameStateEventPayload>(GameStateEventPayload::State::STOPPED);
        App->event->Publish(game_state);

        GameTimer::Stop();

        LoadScene("tmp_scene.scene");
    }
}

bool Hachiko::ModuleSceneManager::IsScenePlaying()
{
    return GameTimer::running && !GameTimer::paused;
}

UpdateStatus Hachiko::ModuleSceneManager::Update(const float delta)
{
    if (scene_ready_to_load)
    {
        scene_ready_to_load = false;
        LoadScene(scene_to_load.c_str());
    }

    main_scene->Update();
    return UpdateStatus::UPDATE_CONTINUE;
}

bool Hachiko::ModuleSceneManager::CleanUp()
{
    SaveScene();
    RELEASE(main_scene);
    return true;
}

//void Hachiko::ModuleSceneManager::LoadModel(const char* model_path) const
//{
//    // delete scene_model;
//    main_scene->LoadFBX(model_path);
//}
//
//void Hachiko::ModuleSceneManager::LoadImageObject(const char* model_path) const
//{
//    std::string file = App->file_sys->GetFileNameAndExtension(model_path);
//    std::string destination = std::string(ASSETS_FOLDER_TEXTURES) + "/" + file;
//    App->file_sys->Copy(model_path, destination.c_str());
//    main_scene->LoadImageObject(destination);
//}

void Hachiko::ModuleSceneManager::CreateEmptyScene()
{
    delete main_scene;
    main_scene = new Scene();
}

void Hachiko::ModuleSceneManager::LoadScene(const char* file_path)
{
    delete main_scene;
    main_scene = serializer.Load(file_path);
    
#ifdef PLAY_BUILD
    App->camera->ReturnPlayerCamera();
    main_scene->Start();
#endif
}

void Hachiko::ModuleSceneManager::SaveScene()
{
    serializer.Save(main_scene);
}

void Hachiko::ModuleSceneManager::SaveScene(const char* file_path)
{
    serializer.Save(main_scene, file_path); // TODO: Take into account temporal scenes
}

void Hachiko::ModuleSceneManager::SwitchTo(const char* file_path)
{
    scene_ready_to_load = true;
    scene_to_load = file_path;
}