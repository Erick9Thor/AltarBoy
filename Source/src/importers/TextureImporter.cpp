#include "core/hepch.h"
#include "TextureImporter.h"

#include "Core/preferences/src/ResourcesPreferences.h"

#include "resources/ResourceTexture.h"
#include "modules/ModuleFileSystem.h"
#include "modules/ModuleTexture.h"

Hachiko::TextureImporter::TextureImporter() 
	: Importer(Importer::Type::TEXTURE)
{
}

void Hachiko::TextureImporter::Import(const char* path)
{
    Hachiko::UID uid = Hachiko::UUID::GenerateUID();
    Resource* texture = ImportTexture(path, uid);
    delete texture;
}

Hachiko::Resource* Hachiko::TextureImporter::Load(const char* path)
{
    if (!std::filesystem::exists(path))
    {
        return nullptr;
    }

    char* file_buffer = App->file_sys->Load(path);
    char* cursor = file_buffer;
    unsigned size_bytes = 0;

    auto texture = new ResourceTexture();

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
    texture->id = App->texture->LoadImg(path);
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

void Hachiko::TextureImporter::Save(const Hachiko::Resource* res)
{
    const ResourceTexture* texture = static_cast<const ResourceTexture*>(res);
    const std::string file_path = GetResourcesPreferences()->GetLibraryPath(Resource::Type::TEXTURE) + texture->GetName();

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

Hachiko::Resource* Hachiko::TextureImporter::ImportTexture(const char* path, UID id)
{
    Hachiko::UID uid = id;
    if (!id)
    {
        uid = Hachiko::UUID::GenerateUID();
    }

    std::filesystem::path texture_path = path;
    ResourceTexture* texture = new ResourceTexture();
    texture->path = path;
    texture->SetName(texture_path.filename().replace_extension().string().c_str());
    texture->min_filter = GL_LINEAR_MIPMAP_LINEAR;
    texture->mag_filter = GL_LINEAR;
    texture->wrap = GL_CLAMP;
    texture->bpp = ilGetInteger(IL_IMAGE_BPP);
    texture->width = ilGetInteger(IL_IMAGE_WIDTH);
    texture->height = ilGetInteger(IL_IMAGE_HEIGHT);
    texture->format = ilGetInteger(IL_IMAGE_FORMAT);

    byte* data = ilGetData();
    texture->data_size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
    texture->data = new byte[texture->data_size];
    memcpy(texture->data, data, texture->data_size);

    texture->GenerateBuffer();

    if (texture != nullptr)
    {
        Save(texture);
    }

    return texture;
}
