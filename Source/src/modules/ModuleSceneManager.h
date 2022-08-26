#pragma once
#include "Module.h"

#include "core/Scene.h"

namespace Hachiko
{
    class GameObject;
    class ComponentCamera;
    class ResourcesPreferences;
    class ResourceScene;
    class ResourceNavMesh;

    class ModuleSceneManager final : public Module
    {
        friend class ModuleDebugMode;
    public:
        ModuleSceneManager() = default;
        ~ModuleSceneManager() override = default;

        // --- Life cycle --- //
        bool Init() override;
        
        void AttemptScenePause();
        void AttemptScenePlay();
        void AttemptSceneStop();
        bool IsScenePlaying();

        void RebuildBatches();
        
        UpdateStatus Update(const float delta) override;
        UpdateStatus PostUpdate(const float delta) override;
        bool CleanUp() override;

        void RemoveGameObject(GameObject* go);
        void RemovedGameObject(GameObject* go);

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

        void ChangeSceneById(UID new_scene_id, bool stop_scene = false);
        
        void SaveScene(const char* file_path = nullptr);

        GameObject* Raycast(const float3& origin, const float3& destination, float3* closest_hit = nullptr, GameObject* parent_filter = nullptr, bool active_only = false);
        GameObject* BoundingRaycast(const float3& origin, const float3& destination, GameObject* parent_filter = nullptr, bool active_only = false);

        void OptionsMenu();

        [[nodiscard]] bool IsLoadingScene() const
        {
            return loading_scene;
        }

        void CheckSceneLoading();

    private:
        void StopScene();
        void LoadScene(UID new_scene_id, bool keep_navmesh = false);
        void ChangeMainScene(Scene* new_scene);
        void LoadEmptyScene();
        void PostLoadScene();
        void ReloadScene();

        void ThreadLoadScene(UID scene_id, bool keep_navmesh);

        // Deletes current resource it it doesnt come from resource manager (for now assume it when id 0)
        void SetSceneResource(ResourceScene* scene);
        void RefreshSceneResource();
        Scene* main_scene = nullptr;
        ResourcesPreferences* preferences = nullptr;


        bool scene_change_requested = false;
        bool scene_reload_requested = false;
        UID scene_to_load_id;
        std::vector<GameObject*> to_remove;

        bool scene_autosave = false;
        ResourceScene* scene_resource;
        ResourceNavMesh* navmesh_resource;

        bool loading_scene = false;
        bool was_scene_playing = false;
        ResourceScene* tmp_resource_scene = nullptr;
        Scene* tmp_loading_scene = nullptr;
        std::thread loading_scene_worker;
    };
} // namespace Hachiko
