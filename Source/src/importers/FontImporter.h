#pragma once

#include "importers/Importer.h"

#include "ft2build.h"
#include "freetype.h"
#include "GLfont.h"

namespace Hachiko
{
    class FontImporter final : public Importer
    {
        friend class ModuleResources;

    public:
        FontImporter();
        ~FontImporter() override;

        void Import(const char* path, YAML::Node& meta) override;
        void ImportWithMeta(const char* path, YAML::Node& meta) override;
        Resource* Load(UID id) override;

        void Save(const Resource* resource) override;

        [[nodiscard]] bool IsImported(const char* path) override
        {
            return false;
        }

    private:
        Resource* ImportFont(const char* path, UID uid);
        FT_Library freetype_lib;
    };
} // namespace Hachiko