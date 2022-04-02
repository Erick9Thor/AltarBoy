#pragma once

#include <assimp/scene.h>
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

        // --- Life cycle Scene --- //
        void Start() const;
        void Update() const;
        void Save(JsonFormatterValue j_scene) const;
        void Load(JsonFormatterValue j_scene);

        // --- GameObject Management --- //
        void AddGameObject(GameObject* new_object, GameObject* parent = nullptr) const;
        void DestroyGameObject(GameObject* game_object) const;
        GameObject* CreateNewGameObject(const char* name, GameObject* parent = nullptr);

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

        [[nodiscard]] GameObject* RayCast(const LineSegment& segment) const;

        void CreateLights();
        std::vector<ComponentDirLight*> dir_lights;
        std::vector<ComponentPointLight*> point_lights;
        std::vector<ComponentSpotLight*> spot_lights;

        // --- Importer --- // TODO: Move to importer
        GameObject* LoadFBX(const std::string& path);

    private:
        // TODO: Fix leaks from resource material pointers when doing import system
        void LoadNode(const aiScene* scene, const aiNode* node, GameObject* parent, const std::string& model_path);
        //std::vector<ResourceMaterial*> LoadMaterials(const aiScene* scene, const std::string& model_path, const std::string& model_name);

    private:
        GameObject* root = nullptr;
        ComponentCamera* culling_camera = nullptr;

        Skybox* skybox;
        Quadtree* quadtree = nullptr;

        bool draw_all_bounding_boxes = false;
    };
}
