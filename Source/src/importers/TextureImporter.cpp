#include "core/hepch.h"
#include "TextureImporter.h"

#include "core/preferences/src/ResourcesPreferences.h"

#include "resources/ResourceTexture.h"
#include "modules/ModuleTexture.h"
#include "modules/ModuleResources.h"

void Hachiko::TextureImporter::Import(const char* path, YAML::Node& meta)
{
    // Only 1 texture will exist
    static const int resource_index = 0;
    UID uid = ManageResourceUID(Resource::Type::TEXTURE, resource_index, meta);
    std::string extension = FileSystem::GetFileExtension(path);
    // TODO: could we extract ModuleTexture functionality to this importer?

    ResourceTexture* texture = App->texture->ImportTextureResource(uid, path, extension != TIF_TEXTURE_EXTENSION);
     
    if (texture != nullptr)
    {
        Save(uid, texture);
    }
    delete texture;
}

Hachiko::Resource* Hachiko::TextureImporter::Load(UID id)
{
    if (!id)
    {
        // No id defaults to no resource
        return nullptr;
    }

    const std::string file_path = GetResourcePath(Resource::Type::TEXTURE, id);

    char* file_buffer = FileSystem::Load(file_path.c_str());
    char* cursor = file_buffer;
    unsigned size_bytes = 0;

    auto texture = new ResourceTexture(id);

    unsigned header[9];
    size_bytes = sizeof(header);
    memcpy(header, cursor, size_bytes);
    cursor += size_bytes;

    unsigned path_size = header[0];
    texture->width = header[1];
    texture->height = header[2];
    texture->format = header[3];
    texture->bpp = header[4];
    texture->min_filter = header[5];
    texture->mag_filter = header[6];
    texture->wrap = header[7];
    texture->data_size = header[8];

    size_bytes = path_size;
    texture->path = "";
    for (unsigned i = 0; i < size_bytes; ++i)
    {
        texture->path += cursor[i];
    }
    cursor += size_bytes;

    size_bytes = texture->data_size;
    texture->data = new byte[size_bytes];
    memcpy(texture->data, cursor, size_bytes);
    cursor += size_bytes;

    texture->GenerateBuffer();

    delete[] file_buffer;

    return texture;
}

void Hachiko::TextureImporter::Save(UID id, const Hachiko::Resource* res)
{
    const ResourceTexture* texture = static_cast<const ResourceTexture*>(res);
    const std::string file_path = GetResourcePath(Resource::Type::TEXTURE, id);

    unsigned header[9] = {
        texture->path.length(),
        texture->width,
        texture->height,
        texture->format,
        texture->bpp,
        texture->min_filter,
        texture->mag_filter,
        texture->wrap,
        texture->data_size
    };

    unsigned file_size = 0;
    file_size += sizeof(header);
    file_size += texture->path.length();
    file_size += texture->data_size;

    const auto file_buffer = new char[file_size];
    char* cursor = file_buffer;
    unsigned size_bytes = 0;

    size_bytes = sizeof(header);
    memcpy(cursor, header, size_bytes);
    cursor += size_bytes;

    size_bytes = texture->path.length();
    memcpy(cursor, texture->path.c_str(), size_bytes);
    cursor += size_bytes;

    size_bytes = texture->data_size;
    memcpy(cursor, texture->data, size_bytes);
    cursor += size_bytes;

    FileSystem::Save(file_path.c_str(), file_buffer, file_size);
    delete[] file_buffer;
}


Hachiko::ResourceTexture* Hachiko::TextureImporter::CreateTextureAssetFromAssimp(const std::string& model_path, const aiMaterial* ai_material, aiTextureType type)
{
    static const int index = 0;
    aiString file;
    aiReturn ai_ret = ai_material->GetTexture(type, index, &file);
    if (ai_ret == AI_FAILURE || ai_ret == AI_OUTOFMEMORY)
    {
        return nullptr;
    }

    ResourceTexture* output_texture = nullptr;
    
    const char* asset_path = App->preferences->GetResourcesPreference()->GetAssetsPath(Resource::AssetType::TEXTURE);
    std::vector<std::string> search_paths;
    const std::string model_texture_path(file.data);
    const std::string filename = model_texture_path.substr(model_texture_path.find_last_of("/\\") + 1);

    std::string texture_path = asset_path + filename;

    search_paths.emplace_back(texture_path);
    search_paths.emplace_back(file.data);

    for (std::string& path : search_paths)
    {
        if (!std::filesystem::exists(path))
        {
            continue;
        }

        UID id = App->resources->ImportAssetFromAnyPath(path)[0];
        output_texture = static_cast<ResourceTexture*>(App->resources->GetResource(Resource::Type::TEXTURE, id)); 
        break;
    }
    return output_texture;
}
