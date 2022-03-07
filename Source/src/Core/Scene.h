#pragma once

#include <string>
#include <vector>

namespace Hachiko
{
    class ModuleSceneManager;
    class GameObject;
    class ComponentTransform;
    class ComponentCamera;
    class ComponentDirLight;
    class ComponentPointLight;
    class ComponentSpotLight;
    class Skybox;
    class Quadtree;

    class Scene
    {
        friend class ModuleSceneManager;

    public:
        Scene();
        ~Scene();
        void CleanScene() const;
        void Update() const;

        // --- GameObject Management --- //
        void AddGameObject(GameObject* new_object, GameObject* parent = nullptr) const;
        void DestroyGameObject(GameObject* game_object) const;
        GameObject* CreateNewGameObject(GameObject* parent = nullptr, const char* name = nullptr);
        [[nodiscard]] GameObject* RayCast(const LineSegment& segment) const;
        [[nodiscard]] GameObject* GetRoot() const
        {
            return root;
        }

        [[nodiscard]] ComponentCamera* GetCullingCamera() const
        {
            return culling_camera;
        }

        void SetCullingCamera(ComponentCamera* v)
        {
            culling_camera = v;
        }

        // --- Quadtree --- //
        [[nodiscard]] Quadtree* GetQuadtree() const
        {
            return quadtree;
        }

        // --- Debug --- //
        GameObject* CreateDebugCamera();

        // --- Skybox --- //
        [[nodiscard]] Skybox* GetSkybox() const
        {
            return skybox;
        }

        [[nodiscard]] const char* GetName() const 
        {
            return name.c_str();
        }

        void SetName(const char* new_name)
        {
            name = new_name;
        }

        // TODO: This hardcoded components can be set in preferences
        void CreateLights();
        std::vector<ComponentDirLight*> dir_lights;
        std::vector<ComponentPointLight*> point_lights;
        std::vector<ComponentSpotLight*> spot_lights;

    private:
        bool draw_all_bounding_boxes = false;
        std::string name;
        GameObject* root = nullptr;
        ComponentCamera* culling_camera = nullptr;
        Skybox* skybox;
        Quadtree* quadtree = nullptr;
    };
}
