#pragma once

namespace Hachiko
{
    class Scene;
    class GameObject;
    class Component;

    class SceneSerializer
    {
        friend ModuleSceneManager;

    public:
        SceneSerializer() = default;
        ~SceneSerializer() = default;

        bool Save(const Scene* scene);
    private:
        // Load is private to ensure only ModuleScene can call it 
        // as we retrieve ownership of scene object pointer
        Scene* Load(const char* path);
        bool CheckIfImported(const char* path) const; // TODO: Move to ImportManager
        ResourcesPreferences* preferences;
    };
}