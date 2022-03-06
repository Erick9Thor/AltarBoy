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
        Resource* Load(const UID id) override;
        void Save(const Resource* resource) override;
    };
}
