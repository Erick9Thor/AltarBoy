#include "core/hepch.h"
#include "TextureImporter.h"
#include "Core/preferences/src/ResourcesPreferences.h"

Hachiko::TextureImporter::TextureImporter() 
	: Importer(Importer::Type::TEXTURE)
{
    preferences = static_cast<ResourcesPreferences*>(App->preferences->GetPreference(Preferences::Type::RESOURCES));
}

void Hachiko::TextureImporter::Import(const char* path)
{
    ILuint size;
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
    }
}

Hachiko::Resource* Hachiko::TextureImporter::Load(const UID id)
{
}

void Hachiko::TextureImporter::Save(const Hachiko::Resource* resource)
{
}
