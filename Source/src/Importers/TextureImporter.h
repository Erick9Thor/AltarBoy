#pragma once

#include "importers/Importer.h"

namespace Hachiko
{

    class TextureImporter final : public Importer
    {
    public:
        TextureImporter();
        ~TextureImporter() override = default;
        void Import(const char* path) override;
        Resource* ImportResource(const char* path);
        Resource* Load(const UID id) override;
        Resource* Load(const char* file_path, const UID& id);
        void Save(const Resource* resource) override;
    };
}
