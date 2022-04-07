#include "core/hepch.h"
#include "ModuleSceneManager.h"

#include "ModuleCamera.h"
#include "ModuleEvent.h"

#include "importers/SceneImporter.h"
#include "ModuleFileSystem.h"

Hachiko::ModuleSceneManager::ModuleSceneManager() = default;

Hachiko::ModuleSceneManager::~ModuleSceneManager() = default;

bool Hachiko::ModuleSceneManager::Init()
{
    HE_LOG("Creating Empty scene");

    // main_scene = new Scene();
    Event scene_load(Event::Type::SCENE_LOADED);
    scene_load.SetEventData<SceneLoadEventPayload>(
        SceneLoadEventPayload::State::NOT_LOADED);
    App->event->Publish(scene_load);

    //main_scene = SceneImporter::LoadScene(ASSETS_FOLDER "/Scenes/lights_delivery.scene");

#ifdef PLAY_BUILD
    main_scene = SceneImporter::LoadScene(ASSETS_FOLDER "/Scenes/menu.scene");
#else
    main_scene = SceneImporter::LoadScene(ASSETS_FOLDER "/Scenes/first_deliver_scene.scene");
#endif

    scene_load.SetEventData<SceneLoadEventPayload>(
        SceneLoadEventPayload::State::LOADED);
    App->event->Publish(scene_load);

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
    RELEASE(main_scene);
    return true;
}

void Hachiko::ModuleSceneManager::LoadModel(const char* model_path) const
{
    // delete scene_model;
    main_scene->LoadFBX(model_path);
}

void Hachiko::ModuleSceneManager::LoadImageObject(const char* model_path) const
{
    std::string file = App->file_sys->GetFileNameAndExtension(model_path);
    std::string destination = std::string(ASSETS_FOLDER_TEXTURES) + "/" + file;
    App->file_sys->Copy(model_path, destination.c_str());
    main_scene->LoadImageObject(destination);
}

void Hachiko::ModuleSceneManager::CreateEmptyScene()
{
    delete main_scene;
    main_scene = new Scene();
}

void Hachiko::ModuleSceneManager::LoadScene(const char* file_path)
{
    Event scene_load(Event::Type::SCENE_LOADED);

    scene_load.SetEventData<SceneLoadEventPayload>(
        SceneLoadEventPayload::State::NOT_LOADED);
    App->event->Publish(scene_load);

    delete main_scene;

    main_scene = SceneImporter::LoadScene(file_path);

#ifdef PLAY_BUILD
    App->camera->ReturnPlayerCamera();
    main_scene->Start();
#endif

    scene_load.SetEventData<SceneLoadEventPayload>(
        SceneLoadEventPayload::State::LOADED);
    App->event->Publish(scene_load);
}

void Hachiko::ModuleSceneManager::SaveScene(const char* file_path) const
{
    SceneImporter::SaveScene(main_scene, file_path);
}

Hachiko::GameObject* Hachiko::ModuleSceneManager::Raycast(const float3& origin, const float3& destination)
{
    return main_scene->RayCast(origin, destination);
}

void Hachiko::ModuleSceneManager::SwitchTo(const char* file_path)
{
    scene_ready_to_load = true;
    scene_to_load = file_path;
}