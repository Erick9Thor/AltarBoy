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

        unsigned min_filter = 0x2703;
        unsigned mag_filter = 0x2601;
        unsigned wrap = 0x2900;
    };
} // namespace Hachiko
