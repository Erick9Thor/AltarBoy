#pragma once
#include "Module.h"

#include "core/Scene.h"

namespace Hachiko
{
    class GameObject;
    class ComponentCamera;

    class ModuleSceneManager final : public Module
    {
    public:
        ModuleSceneManager();
        ~ModuleSceneManager() override;

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
        void SaveScene(const char* file_path) const;

    private:
        Scene* main_scene = nullptr;
    };
}
