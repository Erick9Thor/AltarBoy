#pragma once

#include "importers/Importer.h"

namespace Hachiko
{

    class TextureImporter final : public Importer
    {
        friend class MaterialImporter;
        friend class ModuleResources;

    public:
        TextureImporter();
        ~TextureImporter() override = default;

        void Import(const char* path, UID uid) override;
        Resource* Load(UID id) override;

        void Save(const Resource* resource) override;

    private:
        Resource* ImportTexture(const char* path, UID uid);
    };
}
