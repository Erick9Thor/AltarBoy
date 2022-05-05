#include "core/hepch.h"
#include "FontImporter.h"

#include "modules/ModuleResources.h"
#include "resources/ResourceFont.h"

Hachiko::FontImporter::FontImporter() : Importer(Importer::Type::FONT)
{
    // Initialize fonts library
    if (FT_Init_FreeType(&freetype_lib))
    {
        assert(0 && "Failed to load FreeType library.");
    }
}

Hachiko::FontImporter::~FontImporter()
{
    // Release fonts library
    FT_Done_FreeType(freetype_lib);
}

void Hachiko::FontImporter::Import(const char* path, YAML::Node& meta)
{
    Resource* font = ImportFont(path, meta[GENERAL_NODE][GENERAL_ID].as<UID>());
    delete font;
}

void Hachiko::FontImporter::ImportWithMeta(const char* path, YAML::Node& meta)
{
    Import(path, meta);
}

Hachiko::Resource* Hachiko::FontImporter::Load(UID id)
{
    const std::string file_path = GetResourcesPreferences()->GetLibraryPath(Resource::Type::FONT) + std::to_string(id) + ".ttf";
    ResourceFont* font = new ResourceFont(id);
    font->gl_font = std::make_unique<GLFont>(file_path.c_str(), freetype_lib);

    return font;
}

void Hachiko::FontImporter::Save(const Resource* resource)
{
    const ResourceFont* font = static_cast<const ResourceFont*>(resource);
    // Freetype is dumb and needs the correct extension to work
    const std::string lib_font_path = GetResourcesPreferences()->GetLibraryPath(Resource::Type::FONT) + std::to_string(font->GetID()) + ".ttf";

    // Just use the original font file in lib
    FileSystem::Copy(font->gl_font->getFontFile(), lib_font_path.c_str());
}

Hachiko::Resource* Hachiko::FontImporter::ImportFont(const char* path, UID uid)
{  
    //const std::string asset_file = FileSystem::GetFileNameAndExtension(path);
    //const std::string asset_path = GetResourcesPreferences()->GetAssetsPath(Resource::Type::FONT) + asset_file;
    ResourceFont* font = new ResourceFont(uid);
    font->gl_font = std::make_unique<GLFont>(path, freetype_lib);

    if (font != nullptr)
    {
        Save(font);
    }
    return font;
}
