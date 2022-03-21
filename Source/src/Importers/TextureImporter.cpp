#include "core/hepch.h"
#include "TextureImporter.h"

#include "Core/preferences/src/ResourcesPreferences.h"

#include "resources/ResourceTexture.h"
#include "modules/ModuleFileSystem.h"
#include "modules/ModuleTexture.h"
#include "modules/ModuleResources.h"

Hachiko::TextureImporter::TextureImporter() 
	: Importer(Importer::Type::TEXTURE)
{
}

void Hachiko::TextureImporter::Import(const char* path)
{
    /* ILuint size;
    ILubyte* data;
    ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use
    size = ilSaveL(IL_DDS, nullptr, 0); // Get the size of the data buffer
    if (size > 0)
    {
        data = new ILubyte[size]; // allocate data buffer
        if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveL function
        {
            char* fileBuffer = (char*)data;
        }
    }*/

    auto resource = ImportResource(path);
    delete resource;
}

Hachiko::Resource* Hachiko::TextureImporter::ImportResource(const char* path)
{
    Hachiko::UID uid = Hachiko::UUID::GenerateUID();
    std::string stringPath = path;
    std::string extension = stringPath.substr(stringPath.find_last_of(".") + 1);
    ResourceTexture* texture = App->texture->LoadResource(uid, path, extension != "tif");

    if (texture != nullptr)
    {
        App->resources->textures[uid] = texture;

        Save(texture);
    }
    
    return texture;
}

Hachiko::Resource* Hachiko::TextureImporter::Load(const UID id)
{
    const std::string file_path = GetResourcesPreferences()->GetLibraryPath(Resource::Type::TEXTURE) + std::to_string(id);
    
    return Load(file_path.c_str(), id);
}

Hachiko::Resource* Hachiko::TextureImporter::Load(const char* file_path, const UID& id)
{
    if (!std::filesystem::exists(file_path))
    {
        return nullptr;
    }

    char* file_buffer = App->file_sys->Load(file_path);
    char* cursor = file_buffer;
    unsigned size_bytes = 0;

    const auto texture = new ResourceTexture(id);

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

    char* path = new char[path_size + 1];
    size_bytes = path_size;
    memcpy(path, cursor, size_bytes);
    cursor += size_bytes;
    path[path_size] = '\n';
    texture->path = path;
    delete[] path;

    texture->data = new byte[texture->data_size];
    size_bytes = texture->data_size;
    memcpy(texture->data, cursor, size_bytes);
    cursor += size_bytes;

    texture->GenerateBuffer();

    delete[] file_buffer;

    App->resources->textures[id] = texture;

    return texture;
}

void Hachiko::TextureImporter::Save(const Hachiko::Resource* res)
{
    const ResourceTexture* texture = static_cast<const ResourceTexture*>(res);
    const std::string file_path = GetResourcesPreferences()->GetLibraryPath(Resource::Type::TEXTURE) + std::to_string(texture->GetID());

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

    App->file_sys->Save(file_path.c_str(), file_buffer, file_size);
    delete[] file_buffer;
}
