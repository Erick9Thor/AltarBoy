#pragma once

#include "utils/UUID.h"
#include <utility>

namespace Hachiko
{
    class ModuleSceneManager;
    class GameObject;
    class ComponentTransform;
    class ComponentCamera;
    class ComponentDirLight;
    class ComponentPointLight;
    class ComponentSpotLight;
    class ComponentBillboard;
    class ComponentParticleSystem;
    class Skybox;
    class Quadtree;
    class ResourceMaterial;
    class ResourceNavMesh;
    class BatchManager;

    class Scene
    {
        friend class ModuleSceneManager;

    public:
        Scene();
        ~Scene();

        void CleanScene();

        // --- Life cycle Scene --- //
        void Start() const;
        void Stop() const;
        void Update();

        // --- GameObject Management --- //
        [[nodiscard]] ComponentCamera* GetMainCamera() const;
        GameObject* CreateNewGameObject(GameObject* parent = nullptr, const char* name = nullptr);

        void HandleInputMaterial(ResourceMaterial* material);

        void RebuildBatching();

        [[nodiscard]] GameObject* Raycast(const float3& origin, const float3& destination, float3* closest_hit = nullptr, GameObject* parent_filter = nullptr, bool active_only = false) const;
        [[nodiscard]] GameObject* BoundingRaycast(const float3& origin, const float3& destination, GameObject* parent_filter = nullptr, bool active_only = false) const;
        [[nodiscard]] GameObject* Raycast(const LineSegment& segment, bool triangle_level = true, float3* closest_hit = nullptr, GameObject* parent_filter = nullptr, bool active_only = false) const;

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

        // --- Batching --- //
        void RebuildBatches()
        {
            rebuild_batch = true;
        }

        [[nodiscard]] BatchManager* GetBatchManager() const
        {
            return batch_manager;
        }

        // --- Debug --- //
        GameObject* CreateDebugCamera();

        // --- Skybox --- //
        [[nodiscard]] Skybox* GetSkybox() const
        {
            return skybox;
        }

        [[nodiscard]] bool IsLoaded() const
        {
            return loaded;
        }

        [[nodiscard]] const char* GetName() const
        {
            return name.c_str();
        }

        [[nodiscard]] UID GetNavmeshID() const
        {
            return navmesh_id;
        }

        void SetNavmeshID(UID new_navmesh_id)
        {
            navmesh_id = new_navmesh_id;
        }

        [[nodiscard]] GameObject* Find(UID id) const;

        void SetName(const char* new_name)
        {
            name = new_name;
        }

        float GetAmbientIntensity() const
        {
            return ambient_light_intensity;
        }

        const float4& GetAmbientColor()
        {
            return ambient_light_color;
        }

        bool IsFogEnabled() const
        {
            return fog_enabled;
        }

        const float3& GetFogColor() const
        {
            return fog_color;
        }

        const float* GetFogGlobalDensity() const
        {
            return &fog_global_density;
        }

        const float* GetFogHeightFalloff() const 
        {
            return &fog_height_falloff;
        }

        void Save(YAML::Node& node);
        void Load(const YAML::Node& node, bool meshes_only = false);
        void LoadAmbientParams(const YAML::Node& node);
        void SaveAmbientParams(YAML::Node& node);
        void LoadFogParams(const YAML::Node& node);
        void SaveFogParams(YAML::Node& node);

        void AmbientLightOptionsMenu();
        void FogOptionsMenu();
        void SkyboxOptionsMenu();

        void GetNavmeshData(std::vector<float>& scene_vertices, std::vector<int>& scene_triangles, std::vector<float>& scene_normals, AABB& scene_bounds);

        void AddParticleComponent(Component* new_particle)
        {
            particles.emplace_back(new_particle);
        }

        void RemoveParticleComponent(const UID& component_id);

        const std::vector<Component*>& GetSceneParticles()
        {
            return particles;
        }

        std::vector<ComponentDirLight*> dir_lights{};
        std::vector<ComponentPointLight*> point_lights{};
        std::vector<ComponentSpotLight*> spot_lights{};

    private:
        std::string name;
        GameObject* root = nullptr;
        ComponentCamera* culling_camera = nullptr;
        bool loaded = false;

        UID navmesh_id = 0;

        Skybox* skybox = nullptr;
        Quadtree* quadtree = nullptr;

        bool rebuild_batch = true;
        BatchManager* batch_manager = nullptr;
        std::vector<Component*> particles{};

        // Ambient light params
        float ambient_light_intensity = 0.05f;
        float4 ambient_light_color = float4::one;

        // Fog params
        bool fog_enabled = false;
        float3 fog_color = float3::one;
        float fog_global_density = 0.01f;
        float fog_height_falloff = 0.01f;        

    public:
        class Memento
        {
        public:
            Memento(std::string content) :
                // content(std::move(content))
                content(std::move(content))
            {
            }

            ~Memento() = default;

            [[nodiscard]] std::string GetContent() const
            {
                return content;
            }

        private:
            std::string content;
        };

        [[nodiscard]] Memento* CreateSnapshot();
        void Restore(const Memento*) const;
    };
} // namespace Hachiko
