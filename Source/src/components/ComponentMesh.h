#pragma once
#include "components/Component.h"

#include "resources/ResourceMesh.h"

#include <assimp/scene.h>
#include <MathGeoLib.h>

namespace Hachiko
{
    class GameObject;
    class ComponentCamera;
    class Program;

    class ComponentMesh : public Component
    {
    public:
        ComponentMesh(GameObject* container);
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

        void Import(const aiMesh* mesh);

        void Save(JsonFormatterValue j_component) const override;
        void Load(JsonFormatterValue j_component) override;

        void DrawGui() override;

    private:
        bool visible = true;
        ResourceMesh* resource = nullptr;
    };
}
