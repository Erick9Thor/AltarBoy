#include "core/hepch.h"
#include "ModuleTexture.h"

#include "GLFont.h"
#include "FTLabel.h"

Hachiko::ModuleTexture::ModuleTexture() = default;

Hachiko::ModuleTexture::~ModuleTexture() = default;

bool Hachiko::ModuleTexture::Init()
{
    // Initialize image library
    ilInit();

    // Initialize fonts library
    if (FT_Init_FreeType(&freetype_lib))
    {
        HE_LOG("Failed to load FreeType library.");
        return false;
    }
    
    return true;
}

bool Hachiko::ModuleTexture::CleanUp()
{
    // Release image library
    ilShutDown();

    // Release fonts library
    FT_Done_FreeType(freetype_lib);
    return true;
}

Hachiko::Texture Hachiko::ModuleTexture::Load(const char* path, bool flip)
{
    Texture texture;
    texture.path = path;
    unsigned int img_id = LoadImg(path, flip);

    if (img_id != 0)
    {
        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     ilGetInteger(IL_IMAGE_BPP),
                     texture.width = ilGetInteger(IL_IMAGE_WIDTH),
                     texture.height = ilGetInteger(IL_IMAGE_HEIGHT),
                     0,
                     ilGetInteger(IL_IMAGE_FORMAT),
                     GL_UNSIGNED_BYTE,
                     ilGetData());
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
        DeleteImg(img_id);
        texture.loaded = true; // False by default
    }
    return texture;
}

void Hachiko::ModuleTexture::Unload(Texture& texture)
{
    if (texture.loaded)
    {
        glDeleteTextures(1, &texture.id);
        texture.loaded = false;
    }
}

Hachiko::TextureCube Hachiko::ModuleTexture::LoadCubeMap(const char* paths[6])
{
    constexpr bool flip = true;
    TextureCube cube;
    cube.loaded = true;

    glGenTextures(1, &cube.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube.id);

    // Expected file order x, -x, y, -y, z, -z
    for (int i = 0; i < 6; ++i)
    {
        const unsigned int img_id = LoadImg(paths[i], flip);
        iluFlipImage();
        if (img_id == 0)
        {
            cube.loaded = false;
            continue; // Try loading the other parts despite failing
        }
        // Take advantage of opengl enum with index
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0,
                     ilGetInteger(IL_IMAGE_BPP),
                     cube.widths[i] = ilGetInteger(IL_IMAGE_WIDTH),
                     cube.heighths[i] = ilGetInteger(IL_IMAGE_HEIGHT),
                     0,
                     ilGetInteger(IL_IMAGE_FORMAT),
                     GL_UNSIGNED_BYTE,
                     ilGetData());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return cube;
}

void Hachiko::ModuleTexture::Bind(unsigned id, unsigned slot)
{
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Hachiko::ModuleTexture::Unbind(unsigned slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Hachiko::ModuleTexture::LoadFont()
{
    // TODO: Remove test code
    std::shared_ptr<GLFont> gl_font = std::shared_ptr<GLFont>(new GLFont("assets/fonts/13_5Atom_Sans_Regular.ttf"));
    // Some values we will use to create our labels
    int startX = 100;
    int startY = 100;
    int windowWidth = 800;
    int windowHeight = 600;

    std::unique_ptr<FTLabel> label = std::unique_ptr<FTLabel>(new FTLabel(
        gl_font, // Font face handle
        "Hello world!", // Text to render
        startX,
        startY,
        windowWidth,
        windowHeight));
    /* label->setPosition(500, 250);
    label->setSize(100, 100);
    label->setPixelSize(24);
    label->setIndentation(50);
    label->setAlignment(FTLabel::FontFlags::LeftAligned);
    label->setColor(0.89, 0.26, 0.3, 0.9);
    label->rotate(90, 0, 1, 0); */
}

void SetOption(unsigned option, unsigned value)
{
    glTexParameteri(GL_TEXTURE_2D, option, value);
}

void Hachiko::ModuleTexture::OptionsMenu() const
{
    const char* labels_mag[] = {"Linear", "Nearest"};
    const unsigned values_mag[] = {GL_LINEAR, GL_NEAREST};
    const char* labels_min[] = {"Nearest", "Linear", "Nearest Mipmaps Nearest Criteria", "Nearest Mipmap Linear Criteria", "Linear Mipmaps (Two Closest) Nearest Criteria",
                                "Linear Mipmaps (Two Closest) Linear Criteria"};
    const unsigned values_min[] = {GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR};
    static int mag_filter = 0; // Default is GL_LINEAR
    static int min_filter = 3; // Default is GL_NEAREST_MIPMAP_LINEAR

    if (ImGui::Combo("Mag Filter", &mag_filter, labels_mag, IM_ARRAYSIZE(labels_mag)))
        SetOption(GL_TEXTURE_MAG_FILTER, values_mag[mag_filter]);
    if (ImGui::Combo("Min Filter", &min_filter, labels_min, IM_ARRAYSIZE(labels_min)))
        SetOption(GL_TEXTURE_MIN_FILTER, values_min[min_filter]);
}

unsigned int Hachiko::ModuleTexture::LoadImg(const char* path, bool flip)
{
    ILuint img_id; // The image name to return.
    ilGenImages(1, &img_id); // Grab a new image name.
    ilBindImage(img_id);
    if (!ilLoadImage(path))
        return 0;
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    if (flip)
        iluFlipImage();
    return img_id;
}

void Hachiko::ModuleTexture::DeleteImg(unsigned& img_id)
{
    ilDeleteImages(1, &img_id);
}
