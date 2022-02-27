#include "core/hepch.h"

#include "MeshImporter.h"

#include "modules/ModuleFileSystem.h"
#include "resources/ResourceMesh.h"

Hachiko::ResourceMesh* Hachiko::MeshImporter::Import(const aiMesh* ai_mesh)
{
    const auto r_mesh = new ResourceMesh(UUID::GenerateUID()); // WARNING: This class is delegating the responsability to delete this allocation to the client.
    r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)] = ai_mesh->mNumVertices * 3;
    r_mesh->vertices = new float[r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)]];
    memcpy(r_mesh->vertices, ai_mesh->mVertices, r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)] * sizeof(float));

    // TODO: use mesh->HasFaces(), mesh->HasNormals() and mesh->HasTextureCoords() to load if exists
    r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)] = ai_mesh->mNumVertices * 3;
    r_mesh->normals = new float[r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)]];
    memcpy(r_mesh->normals, ai_mesh->mNormals, r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)] * sizeof(float));

    r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)] = ai_mesh->mNumVertices * 2;
    r_mesh->tex_coords = new float[r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)]];
    for (unsigned i = 0; i < ai_mesh->mNumVertices; i++)
    {
        memcpy(&r_mesh->tex_coords[i * 2], &ai_mesh->mTextureCoords[0][i], 2 * sizeof(unsigned));
    }

    r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)] = ai_mesh->mNumFaces * 3;
    r_mesh->indices = new unsigned[r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)]];
    for (unsigned i = 0; i < ai_mesh->mNumFaces; ++i)
    {
        memcpy(&r_mesh->indices[i * 3], ai_mesh->mFaces[i].mIndices, 3 * sizeof(unsigned));
    }

    r_mesh->GenerateBuffers();
    r_mesh->GenerateAABB();
    r_mesh->loaded = true;

    // Save resource

    return r_mesh;
}

Hachiko::ResourceMesh* Hachiko::MeshImporter::Import(const aiMesh* ai_mesh, const UID id)
{
    const auto r_mesh = new ResourceMesh(id); // WARNING: This class is delegating the responsability to delete this allocation to the client.
    r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)] = ai_mesh->mNumVertices * 3;
    r_mesh->vertices = new float[r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)]];
    memcpy(r_mesh->vertices, ai_mesh->mVertices, r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)] * sizeof(float));

    // TODO: use mesh->HasFaces(), mesh->HasNormals() and mesh->HasTextureCoords() to load if exists
    r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)] = ai_mesh->mNumVertices * 3;
    r_mesh->normals = new float[r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)]];
    memcpy(r_mesh->normals, ai_mesh->mNormals, r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)] * sizeof(float));

    r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)] = ai_mesh->mNumVertices * 2;
    r_mesh->tex_coords = new float[r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)]];
    for (unsigned i = 0; i < ai_mesh->mNumVertices; i++)
    {
        memcpy(&r_mesh->tex_coords[i * 2], &ai_mesh->mTextureCoords[0][i], 2 * sizeof(unsigned));
    }

    r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)] = ai_mesh->mNumFaces * 3;
    r_mesh->indices = new unsigned[r_mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)]];
    for (unsigned i = 0; i < ai_mesh->mNumFaces; ++i)
    {
        memcpy(&r_mesh->indices[i * 3], ai_mesh->mFaces[i].mIndices, 3 * sizeof(unsigned));
    }

    r_mesh->GenerateBuffers();
    r_mesh->GenerateAABB();
    r_mesh->loaded = true;

    // Save resource

    return r_mesh;
}

void Hachiko::MeshImporter::Save(ResourceMesh* r_mesh, UID uid)
{
    const std::string file_path = std::string(LIBRARY_MESH_FOLDER "/") + std::to_string(uid) + ".mesh";

    const unsigned* sizes = r_mesh->buffer_sizes;
    unsigned header[static_cast<int>(ResourceMesh::Buffers::COUNT)] = {sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)], sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)],
                                                                       sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)], sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)],};

    unsigned file_size = 0;
    file_size += sizeof(header);
    file_size += sizeof(unsigned) * sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)];
    file_size += sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)];
    file_size += sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)];
    file_size += sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)];

    const auto file_buffer = new char[file_size];
    char* cursor = file_buffer;
    unsigned size_bytes = 0;

    size_bytes = sizeof(header);
    memcpy(cursor, sizes, size_bytes);
    cursor += size_bytes;

    size_bytes = sizeof(unsigned) * sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)];
    memcpy(cursor, r_mesh->indices, size_bytes);
    cursor += size_bytes;

    size_bytes = sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)];
    memcpy(cursor, r_mesh->vertices, size_bytes);
    cursor += size_bytes;

    size_bytes = sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)];
    memcpy(cursor, r_mesh->normals, size_bytes);
    cursor += size_bytes;

    size_bytes = sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)];
    memcpy(cursor, r_mesh->tex_coords, size_bytes);
    cursor += size_bytes;

    App->file_sys->Save(file_path.c_str(), file_buffer, file_size);
    delete[] file_buffer;
}

Hachiko::ResourceMesh* Hachiko::MeshImporter::Load(UID uid)
{
    const std::string file_path = std::string(LIBRARY_MESH_FOLDER "/") + std::to_string(uid) + ".mesh";
    char* file_buffer = App->file_sys->Load(file_path.c_str());
    char* cursor = file_buffer;
    unsigned size_bytes = 0;

    // TODO: Manage uid properly
    const auto r_mesh = new ResourceMesh(1);

    unsigned header[static_cast<int>(ResourceMesh::Buffers::COUNT)];
    size_bytes = sizeof(header);
    memcpy(header, cursor, size_bytes);
    cursor += size_bytes;

    unsigned* sizes = r_mesh->buffer_sizes;
    sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)] = header[0];
    sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)] = header[1];
    sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)] = header[2];
    sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)] = header[3];

    size_bytes = sizeof(unsigned) * sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)];
    r_mesh->indices = new unsigned[sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)]];
    memcpy(r_mesh->indices, cursor, size_bytes);
    cursor += size_bytes;

    r_mesh->vertices = new float[sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)]];
    size_bytes = sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)];
    memcpy(r_mesh->vertices, cursor, size_bytes);
    cursor += size_bytes;

    r_mesh->normals = new float[sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)]];
    size_bytes = sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)];
    memcpy(r_mesh->normals, cursor, size_bytes);
    cursor += size_bytes;

    r_mesh->tex_coords = new float[sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)]];
    size_bytes = sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)];
    memcpy(r_mesh->tex_coords, cursor, size_bytes);
    cursor += size_bytes;

    r_mesh->GenerateBuffers();
    r_mesh->GenerateAABB();
    r_mesh->loaded = true;

    delete[] file_buffer;

    return r_mesh;
}
