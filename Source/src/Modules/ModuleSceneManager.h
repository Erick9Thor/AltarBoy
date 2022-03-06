#pragma once
#include "Module.h"

#include "core/Scene.h"
#include "Core/serialization/SceneSerializer.h"

namespace Hachiko
{
    class GameObject;
    class ComponentCamera;
    class ResourcesPreferences;

    class ModuleSceneManager final : public Module
    {
    public:
        ModuleSceneManager() = default;
        ~ModuleSceneManager() override = default;

        // --- Life cycle --- //
        bool Init() override;
        UpdateStatus Update(float delta) override;
        bool CleanUp() override;

        // --- Scene management --- //
        void LoadModel(const char* model_path) const; // TODO: delete Change to load scene and load al gameObjects for path

        GameObject* GetRoot()
        {
            return main_scene->root;
        }

        [[nodiscard]] const GameObject* GetRoot() const
        {
            return main_scene->root;
        }

        Scene* GetActiveScene()
        {
            return main_scene;
        }

        [[nodiscard]] const Scene* GetActiveScene() const
        {
            return main_scene;
        }

        void CreateEmptyScene();

        void LoadScene(const char* file_path);
        void SaveScene(const char* file_path);

    private:
        Scene* main_scene = nullptr;
        SceneSerializer serializer;
        ResourcesPreferences* preferences = nullptr;
    };
}
