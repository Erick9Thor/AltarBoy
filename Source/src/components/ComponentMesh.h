#pragma once
#include "Component.h"

#include "resources/ResourceMesh.h"

#include "MathGeoLib.h"

namespace Hachiko
{
    class GameObject;
    class ComponentCamera;
    class Program;

    class ComponentMesh : public Component
    {
    public:
        ComponentMesh(GameObject* container);
        ComponentMesh(GameObject* container, ResourceMesh* res);
        ~ComponentMesh() override;

        void Draw(ComponentCamera* camera, Program* program) override;
        void DrawStencil(ComponentCamera* camera, Program* program) const;

        static Type GetType()
        {
            return Type::MESH;
        }

        [[nodiscard]] bool IsLoaded() const
        {
            return !meshes.empty();
        }

        [[nodiscard]] bool IsVisible() const
        {
            return visible;
        }

        [[nodiscard]] AABB GetAABB(const int index) const
        {
            return meshes[index]->bounding_box;
        }

        [[nodiscard]] unsigned GetBufferSize(const int index, ResourceMesh::Buffers buffer) const
        {
            return meshes[index]->buffer_sizes[static_cast<int>(buffer)];
        }

        [[nodiscard]] unsigned GetBufferId(const int index, ResourceMesh::Buffers buffer) const
        {
            return meshes[index]->buffer_ids[static_cast<int>(buffer)];
        }

        [[nodiscard]] const float* GetVertices(const int index) const
        {
            return meshes[index]->vertices;
        }

        [[nodiscard]] const unsigned* GetIndices(const int index) const
        {
            return meshes[index]->indices;
        }

        [[nodiscard]] const std::string& GetResourcePath() const
        {
            return resource_path;
        }

        [[nodiscard]] size_t GetMeshesCount() const
        {
            return meshes.size();
        }

        [[nodiscard]] ResourceMesh* GetMeshesByIndex(const int index) const
        {
            return meshes[index];
        }

        void LoadModel(const char* model_path);
        void DrawGui() override;

    private:
        bool visible = true;
        std::string resource_path;
        std::vector<ResourceMesh*> meshes = {};
        
        void DisplayNotLoadedUI();
    };
}
