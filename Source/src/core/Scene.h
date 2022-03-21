#pragma once

#include "core/serialization/Serializable.h"

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
    class ResourceModel;
    class ResourceMaterial;

    class Scene : public Serializable
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

        void HandleInputModel(ResourceModel* model);
        void HandleInputMaterial(ResourceMaterial* material);

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

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;
        
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
