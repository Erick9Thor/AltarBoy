#pragma once
#include "Resource.h"

namespace Hachiko
{
    class ResourceMesh : public Resource
    {
    public:
        enum class Buffers
        {
            INDICES = 0,
            VERTICES,
            NORMALS,
            TEX_COORDS,
            TANGENTS,
            COUNT,
        };

        struct Layout
        {
            bool normals = false;
            bool text_coords = false;
        };

        ResourceMesh(UID id);
        ~ResourceMesh() override;
        void GenerateBuffers();
        void GenerateAABB();

        void CleanUp();

        bool loaded = false;

        Layout layout;
        AABB bounding_box;

        // Store the ids of buffers in opengl
        unsigned vbo{}; // TODO: remove this
        unsigned ebo{};
        unsigned vao{};
        unsigned buffer_ids[static_cast<int>(Buffers::COUNT)]{};

        // Track buffer sizes
        unsigned buffer_sizes[static_cast<int>(Buffers::COUNT)]{};

        // In memory buffers
        unsigned* indices{};
        float* vertices{};
        float* normals{};
        float* tex_coords{};
        float* tangents{};
    };

    bool operator==(const ResourceMesh::Layout& l, const ResourceMesh::Layout& r);
}
