#pragma once
#include "Component.h"

#include "resources/ResourceMesh.h"

#include "assimp/scene.h"
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
            return resource->loaded;
        }

        [[nodiscard]] bool IsVisible() const
        {
            return visible;
        }

        [[nodiscard]] AABB GetAABB() const
        {
            return resource->bounding_box;
        }

        [[nodiscard]] unsigned GetBufferSize(ResourceMesh::Buffers buffer) const
        {
            return resource->buffer_sizes[static_cast<int>(buffer)];
        }

        [[nodiscard]] unsigned GetBufferId(ResourceMesh::Buffers buffer) const
        {
            return resource->buffer_ids[static_cast<int>(buffer)];
        }

        [[nodiscard]] const float* GetVertices() const
        {
            return resource->vertices;
        }

        [[nodiscard]] const unsigned* GetIndices() const
        {
            return resource->indices;
        }

        [[nodiscard]] const std::string& GetResourcePath() const
        {
            return resource_path;
        }

        void Import(const aiMesh* mesh); // TODO: nobody besides importers should know assimp

        void Save(JsonFormatterValue j_component) const override;
        void Load(JsonFormatterValue j_component) override;

        void DrawGui() override;

    private:
        bool visible = true;
        std::string resource_path;
        ResourceMesh* resource = nullptr;
    };
}
