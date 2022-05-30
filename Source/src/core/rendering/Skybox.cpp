#include "core/hepch.h"
#include "Skybox.h"

#include "modules/ModuleProgram.h"
#include "components/ComponentCamera.h"
#include "modules/ModuleResources.h"
#include "resources/ResourceTexture.h"
#include "resources/ResourceSkybox.h"

Hachiko::Skybox::Skybox()
{
    cube = ModuleTexture::LoadCubeMap(cube);
    CreateBuffers();
    
}

Hachiko::Skybox::Skybox(TextureCube new_cube) : cube(new_cube)
{
    cube = ModuleTexture::LoadCubeMap(cube);
    CreateBuffers();
}

Hachiko::Skybox::~Skybox()
{
    glDeleteBuffers(1, &vbo);
    ReleaseCubemap();    
}

void Hachiko::Skybox::Draw(ComponentCamera* camera) const
{
    // Use for optimized version (draw at the end) glDepthFunc(GL_LEQUAL);
    OPTICK_CATEGORY("Draw", Optick::Category::Rendering);

    glDepthFunc(GL_ALWAYS);
    Program* program = App->program->GetSkyboxProgram();
    program->Activate();
    // Draw skybox
    glBindVertexArray(vao);


    GLint skybox_binding = glGetUniformLocation(program->GetId(), "skybox");
    glUniform1i(skybox_binding, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube.id);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    Program::Deactivate();
    glDepthFunc(GL_LESS);
}

void Hachiko::Skybox::ReleaseCubemap()
{
    glDeleteTextures(1, &cube.id);
    for (unsigned i = 0; i < static_cast<unsigned>(TextureCube::Side::COUNT); ++i)
    {
        App->resources->ReleaseResource(cube.resources[i]);
    }
    cube.loaded = false;
}

void Hachiko::Skybox::DrawImGui() 
{
    ImGui::PushID(this);

    ImGui::Text("Skybox");

    ImGui::TextDisabled("Keep in mind that all images \nneed to have the same format!");

    if (ImGui::CollapsingHeader("Resources"))
    {
        SelectSkyboxTexture(TextureCube::Side::RIGHT);
        SelectSkyboxTexture(TextureCube::Side::LEFT);
        SelectSkyboxTexture(TextureCube::Side::BOTTOM);
        SelectSkyboxTexture(TextureCube::Side::TOP);
        SelectSkyboxTexture(TextureCube::Side::CENTER);
        SelectSkyboxTexture(TextureCube::Side::BACK);
    }

    ImGui::PopID();
}

const std::string TextureCubeSideToString(Hachiko::TextureCube::Side cube_side)
{
    switch (cube_side)
    {
    case Hachiko::TextureCube::Side::RIGHT:
        return "Right";
    case Hachiko::TextureCube::Side::LEFT:
        return "Left";
    case Hachiko::TextureCube::Side::BOTTOM:
        return "Bottom";
    case Hachiko::TextureCube::Side::TOP:
        return "Top";
    case Hachiko::TextureCube::Side::CENTER:
        return "Center";
    case Hachiko::TextureCube::Side::BACK:
        return "Back";
    }
}

void Hachiko::Skybox::SelectSkyboxTexture(TextureCube::Side cube_side)
{
    const std::string title = Hachiko::StringUtils::Concat("Select texture ", TextureCubeSideToString(cube_side));
    const char* filters = "Image files{.png,.tif,.jpg,.tga}";

    if (ImGui::Button(Hachiko::StringUtils::Concat(TextureCubeSideToString(cube_side).c_str(), " Texture").c_str()))
    {
        ImGuiFileDialog::Instance()->OpenDialog(title.c_str(),
                                                "Select Texture",
                                                filters,
                                                "./assets/skybox/",
                                                1,
                                                nullptr,
                                                ImGuiFileDialogFlags_DontShowHiddenFiles | ImGuiFileDialogFlags_DisableCreateDirectoryButton | ImGuiFileDialogFlags_HideColumnType
                                                    | ImGuiFileDialogFlags_HideColumnDate);
    }

    if (ImGuiFileDialog::Instance()->Display(title.c_str()))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string texture_path = ImGuiFileDialog::Instance()->GetFilePathName();
            texture_path.append(META_EXTENSION);

            YAML::Node texture_node = YAML::LoadFile(texture_path);
            Hachiko::UID res = texture_node[RESOURCES][0][RESOURCE_ID].as<Hachiko::UID>();

            if (res)
            {
                ChangeCubeMapSide(res, cube_side);
            }
            else
            {
                HE_ERROR("Failed when loading the texture.");
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void Hachiko::Skybox::CreateBuffers()
{
    constexpr float vertices[]
        = {
    // positions          

    // Back
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,

    // Left
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    // Right
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    // Front
    1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     

    // Top
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,

    // Bottom
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f
};


    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
}

void Hachiko::Skybox::ChangeCubeMapSide(UID texture_uid, TextureCube::Side cube_side)
{
    unsigned side_number = static_cast<unsigned>(cube_side);
    ReleaseCubemap();
    cube.uids[side_number] = texture_uid;
    cube = ModuleTexture::LoadCubeMap(cube);
}
