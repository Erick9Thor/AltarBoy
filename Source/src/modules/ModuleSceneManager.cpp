#include "core/hepch.h"
#include "ModuleSceneManager.h"

#ifdef PLAY_BUILD
#include "ModuleCamera.h"
#endif

#include "importers/SceneImporter.h"
#include "ModuleFileSystem.h"

Hachiko::ModuleSceneManager::ModuleSceneManager() = default;

Hachiko::ModuleSceneManager::~ModuleSceneManager() = default;

bool Hachiko::ModuleSceneManager::Init()
{
    HE_LOG("Creating Empty scene");

    // main_scene = new Scene();

    //main_scene = SceneImporter::LoadScene(ASSETS_FOLDER "/Scenes/lights_delivery.scene");
    main_scene = SceneImporter::LoadScene(ASSETS_FOLDER "/Scenes/untitled.scene");

    //LoadModel(ASSETS_FOLDER "\\Models\\BakerHouse.fbx"); //TODO: Remove this when importen will be created

    // CreateEmptyScene();
    // LoadScene(LIBRARY_SCENE_FOLDER "/survival_shooter.scene");

#ifdef PLAY_BUILD
    App->camera->SetMainCamera(main_scene->GetMainCamera()); // PLAY_BUILD UNCOMMENT
    main_scene->Start();
#endif

    return true;
}

UpdateStatus Hachiko::ModuleSceneManager::Update(const float delta)
{
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
    delete main_scene;
    main_scene = SceneImporter::LoadScene(file_path);
}

void Hachiko::ModuleSceneManager::SaveScene(const char* file_path) const
{
    SceneImporter::SaveScene(main_scene, file_path);
}
