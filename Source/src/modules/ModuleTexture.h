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
            TOP,
            BOTTOM,
            CENTER,
            BACK,
            COUNT
        };

        static std::string SideString(Side side);
        bool loaded = false;
        unsigned id{};
        UID uids[static_cast<unsigned>(Side::COUNT)] = {0};
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
