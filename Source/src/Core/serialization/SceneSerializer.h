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

        bool Save(const Scene* scene, const char* name, const char* path = nullptr);
    private:
        // Load is private to ensure only ModuleScene can call it 
        // as we retrieve ownership of scene object pointer
        Scene* Load(const char* path);
        YAML::Node SaveEntity(const GameObject* entity);
        YAML::Node SaveComponent(const Component* component) const;
        const GameObject* LoadEntity(YAML::Node entityNode, Scene* scene, GameObject* parent = nullptr);
        void LoadComponent(YAML::Node component, GameObject* entity) const;
        static void ImportFromAssets(const char* path, Resource::Type type);
        bool CheckIfImported(const char* path) const;
        ResourcesPreferences* preferences;
    };
}
