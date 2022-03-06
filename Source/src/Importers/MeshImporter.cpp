#include "core/hepch.h"

#include "MeshImporter.h"
#include "modules/ModuleFileSystem.h"
#include "resources/ResourceMesh.h"
#include "core/preferences/src/ResourcesPreferences.h"

Hachiko::MeshImporter::MeshImporter() : Importer(Importer::Type::MESH)
{
    preferences = static_cast<Hachiko::ResourcesPreferences*>(App->preferences->GetPreference(Hachiko::Preferences::Type::RESOURCES));
}

Hachiko::ResourceMesh* Hachiko::MeshImporter::Import(const aiMesh* ai_mesh)
{
    const auto mesh = new ResourceMesh(UUID::GenerateUID()); // WARNING: This class is delegating the responsability to delete this allocation to the client.
    mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)] = ai_mesh->mNumVertices * 3;
    mesh->vertices = new float[mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)]];
    memcpy(mesh->vertices, ai_mesh->mVertices, mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)] * sizeof(float));

    // TODO: use mesh->HasFaces(), mesh->HasNormals() and mesh->HasTextureCoords() to load if exists
    mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)] = ai_mesh->mNumVertices * 3;
    mesh->normals = new float[mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)]];
    memcpy(mesh->normals, ai_mesh->mNormals, mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)] * sizeof(float));

    mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)] = ai_mesh->mNumVertices * 2;
    mesh->tex_coords = new float[mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)]];
    for (unsigned i = 0; i < ai_mesh->mNumVertices; i++)
    {
        memcpy(&mesh->tex_coords[i * 2], &ai_mesh->mTextureCoords[0][i], 2 * sizeof(unsigned));
    }

    mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)] = ai_mesh->mNumFaces * 3;
    mesh->indices = new unsigned[mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)]];
    for (unsigned i = 0; i < ai_mesh->mNumFaces; ++i)
    {
        memcpy(&mesh->indices[i * 3], ai_mesh->mFaces[i].mIndices, 3 * sizeof(unsigned));
    }

    mesh->GenerateBuffers();
    mesh->GenerateAABB();
    mesh->loaded = true;

    // Save resource

    return mesh;
}

Hachiko::ResourceMesh* Hachiko::MeshImporter::Import(const aiMesh* ai_mesh, const UID id)
{
    const auto mesh = new ResourceMesh(id); // WARNING: This class is delegating the responsability to delete this allocation to the client.
    mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)] = ai_mesh->mNumVertices * 3;
    mesh->vertices = new float[mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)]];
    memcpy(mesh->vertices, ai_mesh->mVertices, mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)] * sizeof(float));

    // TODO: use mesh->HasFaces(), mesh->HasNormals() and mesh->HasTextureCoords() to load if exists
    mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)] = ai_mesh->mNumVertices * 3;
    mesh->normals = new float[mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)]];
    memcpy(mesh->normals, ai_mesh->mNormals, mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)] * sizeof(float));

    mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)] = ai_mesh->mNumVertices * 2;
    mesh->tex_coords = new float[mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)]];
    for (unsigned i = 0; i < ai_mesh->mNumVertices; i++)
    {
        memcpy(&mesh->tex_coords[i * 2], &ai_mesh->mTextureCoords[0][i], 2 * sizeof(unsigned));
    }

    mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)] = ai_mesh->mNumFaces * 3;
    mesh->indices = new unsigned[mesh->buffer_sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)]];
    for (unsigned i = 0; i < ai_mesh->mNumFaces; ++i)
    {
        memcpy(&mesh->indices[i * 3], ai_mesh->mFaces[i].mIndices, 3 * sizeof(unsigned));
    }

    mesh->GenerateBuffers();
    mesh->GenerateAABB();
    mesh->loaded = true;

    // Save resource

    return mesh;
}

void Hachiko::MeshImporter::Save(const Resource* res)
{
    ResourceMesh* mesh = static_cast<ResourceMesh*>(res);
    const std::string file_path = preferences->GetLibraryPath(Resource::Type::MESH) + std::to_string(mesh->GetID());

    const unsigned* sizes = mesh->buffer_sizes;
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
    memcpy(cursor, mesh->indices, size_bytes);
    cursor += size_bytes;

    size_bytes = sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)];
    memcpy(cursor, mesh->vertices, size_bytes);
    cursor += size_bytes;

    size_bytes = sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)];
    memcpy(cursor, mesh->normals, size_bytes);
    cursor += size_bytes;

    size_bytes = sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)];
    memcpy(cursor, mesh->tex_coords, size_bytes);
    cursor += size_bytes;

    App->file_sys->Save(file_path.c_str(), file_buffer, file_size);
    delete[] file_buffer;
}

Hachiko::Resource* Hachiko::MeshImporter::Load(const UID uid)
{
    const std::string file_path = preferences->GetLibraryPath(Resource::Type::MESH) + std::to_string(uid);
    if (!std::filesystem::exists(file_path.c_str()))
    {
        return nullptr;
    }

    char* file_buffer = App->file_sys->Load(file_path.c_str());
    char* cursor = file_buffer;
    unsigned size_bytes = 0;

    // TODO: Manage uid properly
    const auto mesh = new ResourceMesh(1);

    unsigned header[static_cast<int>(ResourceMesh::Buffers::COUNT)];
    size_bytes = sizeof(header);
    memcpy(header, cursor, size_bytes);
    cursor += size_bytes;

    unsigned* sizes = mesh->buffer_sizes;
    sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)] = header[0];
    sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)] = header[1];
    sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)] = header[2];
    sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)] = header[3];

    size_bytes = sizeof(unsigned) * sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)];
    mesh->indices = new unsigned[sizes[static_cast<int>(ResourceMesh::Buffers::INDICES)]];
    memcpy(mesh->indices, cursor, size_bytes);
    cursor += size_bytes;

    mesh->vertices = new float[sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)]];
    size_bytes = sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::VERTICES)];
    memcpy(mesh->vertices, cursor, size_bytes);
    cursor += size_bytes;

    mesh->normals = new float[sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)]];
    size_bytes = sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::NORMALS)];
    memcpy(mesh->normals, cursor, size_bytes);
    cursor += size_bytes;

    mesh->tex_coords = new float[sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)]];
    size_bytes = sizeof(float) * sizes[static_cast<int>(ResourceMesh::Buffers::TEX_COORDS)];
    memcpy(mesh->tex_coords, cursor, size_bytes);
    cursor += size_bytes;

    mesh->GenerateBuffers();
    mesh->GenerateAABB();
    mesh->loaded = true;

    delete[] file_buffer;

    return mesh;
}
