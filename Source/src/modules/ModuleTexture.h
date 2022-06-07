#pragma once
#include "Module.h"

#include <il.h>
#include <string>

#include "freetype.h"
#include "GLfont.h"

namespace Hachiko
{
    class ResourceSkybox;
    class ResourceTexture;
    
    // TODO: Change to proper resource skybox
    // Curent workaround is to use the 6 textures as if they were resource skybox (they are for now)
    struct TextureCube
    {
        enum class Side
        {
            RIGHT,
            LEFT,
            // Order between top and bottom is swaped from opengl
            // Because we flip the y to correct not properly flipped resources
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
        ResourceSkybox* resources[static_cast<unsigned>(Side::COUNT)] = {nullptr};
    };

    struct Font
    {
        bool loaded = false;
        std::string path;
        std::shared_ptr<GLFont> gl_font = nullptr;
    };

    

    class ModuleTexture final : public Module
    {
    public:
        ModuleTexture();
        ~ModuleTexture() override;

        bool Init() override;
        bool CleanUp() override;

        static ResourceTexture* ImportTextureResource(UID uid, const char* path, bool flip = true);
        static ResourceSkybox* ImportSkyboxResource(UID uid, const char* path, bool flip = true);

        static TextureCube LoadCubeMap(TextureCube& cube);
        static void Bind(unsigned id, unsigned slot);
        static void Unbind(unsigned slot);

        Font LoadFont(const char* path);

        [[nodiscard]] short GetDevilVersion() const
        {
            return devil_version;
        }

        void OptionsMenu() const;

        static ::byte* GetData();
        static unsigned int LoadImg(const char* path, bool flip = true);
        static void DeleteImg(unsigned& img_id);

    private:
        const short devil_version = IL_VERSION;        

        FT_Library freetype_lib;
    };
} // namespace Hachiko