#include "core/hepch.h"

#include "SkyboxImporter.h"

#include "resources/ResourceSkybox.h"

void Hachiko::SkyboxImporter::Import(const char* path, YAML::Node& meta)
{
    // Only 1 texture will exist
    static const int resource_index = 0;
    UID uid = ManageResourceUID(Resource::Type::SKYBOX, resource_index, meta);

    ResourceSkybox* texture = App->texture->ImportSkyboxResource(uid, path, false);

    if (texture != nullptr)
    {
        Save(uid, texture);
    }
    delete texture;
}

Hachiko::Resource* Hachiko::SkyboxImporter::Load(UID id)
{
    const std::string file_path = GetResourcePath(Resource::Type::SKYBOX, id);

    if (!FileSystem::Exists(file_path.c_str()))
    {
        return nullptr;
    }

    char* file_buffer = FileSystem::Load(file_path.c_str());
    char* cursor = file_buffer;
    unsigned size_bytes = 0;

    auto texture = new ResourceSkybox(id);

    unsigned header[9];
    size_bytes = sizeof(header);
    memcpy(header, cursor, size_bytes);
    cursor += size_bytes;

    texture->width = header[0];
    texture->height = header[1];
    texture->format = header[2];
    texture->bpp = header[3];
    texture->min_filter = header[4];
    texture->mag_filter = header[5];
    texture->wrap = header[6];
    texture->data_size = header[7];

    size_bytes = texture->data_size;
    texture->data = new byte[size_bytes];
    memcpy(texture->data, cursor, size_bytes);
    cursor += size_bytes;

    texture->GenerateBuffer();

    delete[] file_buffer;

    return texture;
}

void Hachiko::SkyboxImporter::Save(UID id, const Resource* resource)
{
    const ResourceSkybox* texture = static_cast<const ResourceSkybox*>(resource);
    const std::string file_path = GetResourcePath(Resource::Type::SKYBOX, id);

    unsigned header[9] = {texture->width, texture->height, texture->format, texture->bpp, texture->min_filter, texture->mag_filter, texture->wrap, texture->data_size};

    unsigned file_size = 0;
    file_size += sizeof(header);
    file_size += texture->data_size;

    const auto file_buffer = new char[file_size];
    char* cursor = file_buffer;
    unsigned size_bytes = 0;

    size_bytes = sizeof(header);
    memcpy(cursor, header, size_bytes);
    cursor += size_bytes;

    size_bytes = texture->data_size;
    memcpy(cursor, texture->data, size_bytes);
    cursor += size_bytes;

    FileSystem::Save(file_path.c_str(), file_buffer, file_size);
    delete[] file_buffer;
}
