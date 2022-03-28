#pragma once

#include "importers/Importer.h"

namespace Hachiko
{

    class TextureImporter final : public Importer
    {
        friend class MaterialImporter;

    public:
        TextureImporter();
        ~TextureImporter() override = default;
        void Import(const char* path) override;
        Resource* Load(const UID id) override;
        void Save(const Resource* resource) override;

    private:
        Resource* ImportTexture(const char* path, UID id = 0);
    };
}
