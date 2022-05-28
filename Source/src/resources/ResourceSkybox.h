#pragma once
#include "Resource.h"

#include "modules/ModuleTexture.h"

namespace Hachiko
{
    class ResourceSkybox : public Resource
    {
    public:
        ResourceSkybox();
        ResourceSkybox(UID id);
        ~ResourceSkybox() override;

        void GenerateBuffer();

        [[nodiscard]] unsigned GetImageId() const
        {
            return id;
        }
        
        unsigned id = 0;
        unsigned width = 0;
        unsigned height = 0;
        unsigned format = 0;
        unsigned bpp = 0;

        unsigned min_filter = 0x2703;
        unsigned mag_filter = 0x2601;
        unsigned wrap = 0x2900;

        unsigned char* data = nullptr;
        unsigned data_size = 0;
    };
} // namespace Hachiko
