#pragma once

namespace Hachiko
{
    class Importer;

    class TextureImporter final : public Importer
    {
    public:
        TextureImporter();
        ~TextureImporter() override = default;
        void Import(const char* path) override;
        void Load(UID id) override;
        void Save() override;
    };
}
