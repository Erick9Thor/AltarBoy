#pragma once
#include "Resource.h"

#include "modules/ModuleTexture.h"

namespace Hachiko
{
    class ResourceTexture : public Resource
    {
    public:
        ResourceTexture(UID uid);
        ~ResourceTexture() override;

        void GenerateBuffer();

        void DrawGui();

        [[nodiscard]] unsigned GetId() const
        {
            return id;
        }

        unsigned id = 0;
        std::string path;
        unsigned width = 0;
        unsigned height = 0;
        unsigned format = 0;
        unsigned bpp = 0;

        // TODO: Config will be lost in batching
        unsigned min_filter = 0x2703;
        unsigned mag_filter = 0x2601;
        unsigned wrap = 0x2900;

        byte* data{};
        unsigned data_size = 0;
    };
} // namespace Hachiko
