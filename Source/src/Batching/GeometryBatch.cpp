#include "core/hepch.h"
#include "GeometryBatch.h"

#include "../Components/ComponentMesh.h"

Hachiko::GeometryBatch::GeometryBatch(ResourceMesh::Layout batch_layout) {
    batch = new ResourceMesh(0);
    batch->layout = batch_layout;
}

Hachiko::GeometryBatch::~GeometryBatch() {
    RELEASE(batch);
}

void Hachiko::GeometryBatch::AddMesh(const ComponentMesh* mesh)
{
    components.push_back(mesh);

    const ResourceMesh* resource = mesh->GetResource();
    auto it = resources_index.find(resource);

    if (it == resources_index.end())
    {
        resources_index[resource] = 0;
    }
}

void Hachiko::GeometryBatch::GenerateBatch() {
    unsigned resource_index = 0;

    static const int buffer_types[] = {
        static_cast<int>(ResourceMesh::Buffers::INDICES), 
        static_cast<int>(ResourceMesh::Buffers::VERTICES), 
        static_cast<int>(ResourceMesh::Buffers::NORMALS), 
        static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)};

    // Index all resources and compute total size to generate batch buffers;
    for (auto& resource : resources_index)
    {
        const ResourceMesh* r = resource.first;
        resource.second = resource_index;
        ++resource_index;

        for (int buffer_type : buffer_types)
        {
            batch->buffer_sizes[buffer_type] += r->buffer_sizes[buffer_type];
        }
            
    }

    batch->indices = new unsigned[batch->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)]];
    batch->vertices = new float[batch->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)]];
    batch->normals = new float[batch->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)]];
    batch->tex_coords = new float[batch->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)]];

    unsigned indices_offset = 0;
    unsigned vertices_offset = 0;
    unsigned normals_offset = 0;
    unsigned tex_coords_offset = 0;

    for (auto& resource : resources_index)
    {
        const ResourceMesh* r = resource.first;

        unsigned size = r->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)];
        unsigned size_bytes = sizeof(unsigned) * size;
        memcpy(&batch->indices[indices_offset], r->indices, size_bytes);
        indices_offset += size;

        size = r->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)];
        size_bytes = sizeof(float) * size;
        memcpy(&batch->vertices[vertices_offset], r->vertices, size_bytes);
        vertices_offset += size;

        size = r->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)];
        size_bytes = sizeof(float) * size;
        memcpy(&batch->normals[normals_offset], r->normals, size_bytes);
        normals_offset += size;

        size = r->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)];
        size_bytes = sizeof(float) * size;
        memcpy(&batch->tex_coords[tex_coords_offset], r->tex_coords, size_bytes);
        tex_coords_offset += size;        
    }
}

void Hachiko::GeometryBatch::GenerateBuffers() {
    batch->GenerateBuffers();
}
