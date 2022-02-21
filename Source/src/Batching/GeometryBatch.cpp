#include "core/hepch.h"
#include "GeometryBatch.h"

#include "../Modules/ModuleProgram.h"
#include "../Components/ComponentMesh.h"
#include "../Components/ComponentTransform.h"

Hachiko::GeometryBatch::GeometryBatch(ResourceMesh::Layout batch_layout) {
    batch = new ResourceMesh(0);
    batch->layout = batch_layout;
}

Hachiko::GeometryBatch::~GeometryBatch() {
    for (auto& resource : resources)
    {
        delete resource.second;
    }

    if (batch->loaded)
    {
        glDeleteBuffers(1, &instance_indices_vbo);
    }

    RELEASE(batch);
}

void Hachiko::GeometryBatch::AddMesh(const ComponentMesh* mesh)
{
    components.push_back(mesh);

    const ResourceMesh* resource = mesh->GetResource();
    auto it = resources.find(resource);

    if (it == resources.end())
    {
        // It will be formatted when generating batches
        resources[resource] = new DrawCommand();
    }
}

void Hachiko::GeometryBatch::GenerateBatch() {

    BatchMeshes();
    BatchTransforms();
    GenerateCommands();
}

void Hachiko::GeometryBatch::BatchMeshes()
{
    unsigned resource_index = 0;

    static const int buffer_types[] = {static_cast<int>(ResourceMesh::Buffers::INDICES),
                                       static_cast<int>(ResourceMesh::Buffers::VERTICES),
                                       static_cast<int>(ResourceMesh::Buffers::NORMALS),
                                       static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)};

    // Index all resources and compute total size to generate batch buffers;
    for (auto& resource : resources)
    {
        const ResourceMesh* r = resource.first;

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

    for (auto& resource : resources)
    {
        const ResourceMesh* r = resource.first;
        DrawCommand* command = resource.second;

        command->count = r->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)];
        command->first_index = indices_offset;
        command->base_vertex = static_cast<int>(vertices_offset / 3);

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

void Hachiko::GeometryBatch::BatchTransforms()
{
    transforms.reserve(components.size());
    for (auto& component : components)
    {
        transforms.push_back(component->GetGameObject()->GetTransform());
    }
}


void Hachiko::GeometryBatch::GenerateBuffers()
{
    glGenVertexArrays(1, &batch->vao);
    glBindVertexArray(batch->vao);

    // Positions (3 values per coord)
    glGenBuffers(1, &batch->buffer_ids[static_cast<int>(ResourceMesh::Buffers::VERTICES)]);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, batch->buffer_ids[static_cast<int>(ResourceMesh::Buffers::VERTICES)]);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, batch->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)] * sizeof(float), batch->vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    // Normals (3 values per coord)
    glGenBuffers(1, &batch->buffer_ids[static_cast<int>(ResourceMesh::Buffers::NORMALS)]);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, batch->buffer_ids[static_cast<int>(ResourceMesh::Buffers::NORMALS)]);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, batch->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)] * sizeof(float), batch->normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(1);

    // Texture Coords (2 values per coord)
    glGenBuffers(1, &batch->buffer_ids[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)]);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, batch->buffer_ids[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)]);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, batch->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)] * sizeof(float), batch->tex_coords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(2);

    // Instance indices (one per component to draw)
    std::vector<unsigned> indices_vbo(components.size());
    std::iota(std::begin(indices_vbo), std::end(indices_vbo), 0);

    glGenBuffers(1, &instance_indices_vbo);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, instance_indices_vbo);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, components.size() * sizeof(unsigned), indices_vbo.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(unsigned), static_cast<void*>(nullptr));
    glVertexAttribDivisor(3, 1); // advances divisor times per instance/draw command
    glEnableVertexAttribArray(3);

    // Indices (1 value)
    glGenBuffers(1, &batch->buffer_ids[static_cast<int>(ResourceMesh::Buffers::INDICES)]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->buffer_ids[static_cast<int>(ResourceMesh::Buffers::INDICES)]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, batch->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)] * sizeof(unsigned), batch->indices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    batch->loaded = true;
}

void Hachiko::GeometryBatch::BindTransforms(unsigned ssbo_id)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_id);
    glBufferData(GL_SHADER_STORAGE_BUFFER, transforms.size() * sizeof(float) * 16, &transforms[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Hachiko::GeometryBatch::GenerateCommands() {
    commands.reserve(components.size());
    
    unsigned instance = 0;
    for (const ComponentMesh* component : components)
    {
        const ResourceMesh* r = component->GetResource();
        DrawCommand command = (*resources[r]);
        command.base_instance = instance;
        commands.emplace_back(command);
        ++instance;
    }
}

void Hachiko::GeometryBatch::Bind()
{
    glBindVertexArray(batch->vao);
    App->program->UpdateTransforms(transforms);
}
