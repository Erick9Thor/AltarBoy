#pragma once
#include "Module.h"

#include <il.h>
#include <string>

namespace Hachiko
{
    class ResourceTexture;
    
    struct Texture // TODO: removed
    {
        bool loaded = false;
        unsigned id = 0;
        std::string path;
        unsigned width = 0;
        unsigned height = 0;
    };

    struct TextureCube
    {
        enum class Side
        {
            RIGHT,
            LEFT,
            // Order between top and bottom is swaped from opengl
            // Because we flip the y to correct not properly flipped resources
            BOTTOM,
            TOP,            
            CENTER,
            BACK,
            COUNT
        };

        static std::string SideString(Side side);
        bool loaded = false;
        unsigned id{};
        UID uids[static_cast<unsigned>(Side::COUNT)] = {10505613530091827897, 13700979212544735236, 5133574444860088995, 4391007458909829677, 10188972483801135247, 17476158828083105361};
        ResourceTexture* resources[static_cast<unsigned>(Side::COUNT)] = {nullptr};
    };
    
    class ResourceTexture;

    class ModuleTexture final : public Module
    {
    public:
        ModuleTexture();
        ~ModuleTexture() override;

        bool Init() override;
        bool CleanUp() override;

        static ResourceTexture* ImportTextureResource(UID uid, const char* path, bool flip = true);

        [[deprecated]]
        static Texture Load(const char* path, bool flip = true);
        [[deprecated]]
        static void Unload(Texture& texture);
        static TextureCube LoadCubeMap(TextureCube& cube);
        static void Bind(unsigned id, unsigned slot);
        static void Unbind(unsigned slot);

        [[nodiscard]] short GetDevilVersion() const
        {
            return devil_version;
        }

        void OptionsMenu() const;

    private:
        const short devil_version = IL_VERSION;
        static unsigned int LoadImg(const char* path, bool flip = true);
        static void DeleteImg(unsigned& img_id);
    };
}
