#include "core/hepch.h"

#include "ComponentBillboard.h"

#include <imgui_color_gradient.h>

#include "modules/ModuleProgram.h"
#include "modules/ModuleResources.h"
#include "modules/ModuleUserInterface.h"

#include "modules/ModuleCamera.h" // TODO: This is only for debug

#include "resources/ResourceTexture.h"

#include "ComponentTransform.h"
#include "ComponentCamera.h"

Hachiko::ComponentBillboard::ComponentBillboard(GameObject* container, UID id) 
	: Component(Component::Type::BILLBOARD, container, id) 
{
    gradient = new ImGradient();
    CreateSquare();
}

Hachiko::ComponentBillboard::~ComponentBillboard() 
{
    delete gradient;
}

void Hachiko::ComponentBillboard::Draw(ComponentCamera* camera, Program* program)
{
    Program* billboard_program = App->program->GetBillboardProgram();

    if (billboard_program == nullptr)
    {
        return;
    }

    billboard_program->Activate();

    glActiveTexture(GL_TEXTURE0);
    int glTexture = 0;
    //if (texture != nullptr)
    //{
    //    glTexture = texture->GetImageId();
    //    Hachiko::ModuleTexture::Bind(glTexture, static_cast<int>(Hachiko::ModuleProgram::TextureSlots::DIFFUSE));
    //}
    int hasDiffuseMap = texture ? 1 : 0;

    //glDepthMask(GL_FALSE);
    //glEnable(GL_BLEND);
    //glBlendEquation(GL_FUNC_ADD);
    //if (renderMode == BillboardRenderMode::B_ADDITIVE)
    //{
    //    glBlendFunc(GL_ONE, GL_ONE);
    //}
    //else
    //{
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //}
    glBindVertexArray(vao);
    ComponentTransform* transform = GetGameObject()->GetComponent<ComponentTransform>();

    float3x3 rotatePart = transform->GetGlobalMatrix().RotatePart();
    float3 position = transform->GetGlobalPosition();
    float4x4 modelMatrix = transform->GetGlobalMatrix();
    float3 scale = transform->GetGlobalScale();

    Frustum* frustum = camera->GetFrustum();
    float4x4* proj = &camera->GetProjectionMatrix();
    float4x4* view = &camera->GetViewMatrix();

    float4x4 newModelMatrix;

    if (billboardType == BillboardType::NORMAL)
    {
        newModelMatrix = modelMatrix.LookAt(rotatePart.Col(2), -frustum->Front(), rotatePart.Col(1), float3::unitY);
        newModelMatrix = float4x4::FromTRS(position, newModelMatrix.RotatePart() * rotatePart, scale);
    }
    else if (billboardType == BillboardType::STRETCH)
    {
        float3 cameraPos = camera->GetFrustum()->Pos();
        float3 cameraDir = (cameraPos - position).Normalized();
        float3 upDir = Cross(direction, cameraDir);
        float3 newCameraDir = Cross(direction, upDir);

        float3x3 newRotation;
        newRotation.SetCol(0, upDir);
        newRotation.SetCol(1, direction);
        newRotation.SetCol(2, newCameraDir);

        newModelMatrix = float4x4::FromTRS(position, newRotation * modelStretch, scale);
    }
    else if (billboardType == BillboardType::HORIZONTAL)
    {
        if (isHorizontalOrientation)
        {
            float3 direction = transform->GetGlobalRotation().WorldZ();
            float3 projection = position + direction - direction.y * float3::unitY;
            direction = (projection - position).Normalized();
            float3 right = Cross(float3::unitY, direction);

            float3x3 newRotation;
            newRotation.SetCol(1, right);
            newRotation.SetCol(2, float3::unitY);
            newRotation.SetCol(0, direction);

            newModelMatrix = float4x4::FromTRS(position, newRotation, scale);
        }
        else
        {
            newModelMatrix = float4x4::LookAt(float3::unitZ, float3::unitY, float3::unitY, float3::unitY);
            newModelMatrix = float4x4::FromTRS(position, newModelMatrix.RotatePart(), scale);
        }
    }
    else if (billboardType == BillboardType::VERTICAL)
    {
        float3 cameraPos = camera->GetFrustum()->Pos();
        float3 cameraDir = (float3(cameraPos.x, position.y, cameraPos.z) - position).Normalized();
        newModelMatrix = float4x4::LookAt(float3::unitZ, cameraDir, float3::unitY, float3::unitY);
        newModelMatrix = float4x4::FromTRS(position, newModelMatrix.RotatePart(), scale);
    }

    float4x4 identity = float4x4::identity;
    billboard_program->BindUniformFloat4x4("model", identity.ptr()); //newModelMatrix.ptr());
    billboard_program->BindUniformFloat4x4("view", identity.ptr()); //view->ptr());
    billboard_program->BindUniformFloat4x4("proj", identity.ptr()); //proj->ptr());

    float4 color = float4::one;
    if (colorOverLifetime)
    {
        gradient->getColorAt(colorFrame, color.ptr());
    }

    //program->BindUniformInts("diffuseMap", 1, &glTexture);
    //program->BindUniformInts("hasDiffuseMap", 1, &hasDiffuseMap);
    //program->BindUniformFloat4("inputColor", color.ptr());
    //program->BindUniformFloat3("intensity", textureIntensity.ptr());
    //program->BindUniformFloat("currentFrame", &currentFrame);
    //program->BindUniformInts("Xtiles", 1, &Xtiles);
    //program->BindUniformInts("Ytiles", 1, &Ytiles);

    //int flip_x = flipTexture[0] ? 1 : 0;
    //int flip_y = flipTexture[1] ? 1 : 0;
    //program->BindUniformInts("flipX", 1, &flip_x);
    //program->BindUniformInts("flipY", 1, &flip_y);

    glUniform1i(glGetUniformLocation(billboard_program->GetId(), "diffuseMap"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    
    // Clear
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDepthFunc(GL_LESS);
    Program::Deactivate();
    //glDisable(GL_BLEND);
    //glDepthMask(GL_TRUE);
}

void Hachiko::ComponentBillboard::DrawGui()
{
    ImGui::NewLine();
    ImGui::Separator;
    ImGui::Text("Billboard");

    static const ImGuiTreeNodeFlags texture_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

    	// Color Over Lifetime
    if (ImGui::CollapsingHeader("Color over lifetime"))
    {
        ImGui::Checkbox("##color_over_lifetime", &colorOverLifetime);
        if (colorOverLifetime)
        {
            ImGui::SameLine();
            ImGui::PushItemWidth(200);
            ImGui::GradientEditor(gradient, draggingGradient, selectedGradient);
            //ImGui::PushItemWidth(ITEM_SIZE);
            ImGui::NewLine();
            //ImGui::DragFloat("Cycles##color_cycles", &colorCycles, App->editor->dragSpeed2f, 1, inf);
            ImGui::Checkbox("Loop##color_loop", &colorLoop);
        }
    }

    if (ImGui::TreeNodeEx((void*)&texture, texture_flags, "Texture"))
    {
        if (texture != nullptr)
        {
            ImGui::Image(reinterpret_cast<void*>(texture->GetImageId()), ImVec2(80, 80));
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text("%dx%d", texture->width, texture->height);
            ImGui::Text("Path: %s", texture->path.c_str());

            // TODO: textue configuration (maybe delegate to the ResourceTexture)

            ImGui::EndGroup();
        }
        else
        {
            AddTexture();
        }
        ImGui::TreePop();
    }
}

void Hachiko::ComponentBillboard::Update() 
{
}

void Hachiko::ComponentBillboard::Save(YAML::Node& node) const
{
    if (texture != nullptr)
    {
        node[BILLBOARD_TEXTURE_ID] = texture->GetID();
    }
}

void Hachiko::ComponentBillboard::Load(const YAML::Node& node) 
{
    UID texture_id = node[BILLBOARD_TEXTURE_ID].IsDefined() ? node[BILLBOARD_TEXTURE_ID].as<UID>() : 0;
    if (texture_id)
    {
        texture = static_cast<ResourceTexture*>(
            App->resources->GetResource(Resource::Type::TEXTURE, texture_id));
    }
}

void Hachiko::ComponentBillboard::AddTexture() 
{
    const std::string title = "Select billboard texture ";
    ResourceTexture* res = nullptr;

    if (ImGui::Button("Add Texture"))
    {
        ImGuiFileDialog::Instance()->OpenDialog(title.c_str(),
                                                "Select Texture",
                                                ".png,.tif,.jpg,.tga",
                                                "./assets/textures/",
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
            res = static_cast<ResourceTexture*>(App->resources->GetResource(Resource::Type::TEXTURE, texture_node[RESOURCES][0][RESOURCE_ID].as<UID>()));
        }

        ImGuiFileDialog::Instance()->Close();
    }
    if (res != nullptr)
    {
        texture = res;
        textureID = texture->GetID();
    }
}

void Hachiko::ComponentBillboard::RemoveTexture()
{
    texture = nullptr;
}

void Hachiko::ComponentBillboard::CreateSquare()
{
    float positions[] = {
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f // top left
    };

    unsigned int indices[] = {2, 1, 0, 0, 3, 2};

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //HE_LOG("QUAD VAO %d %d %d", vao, vbo, ebo);
    glBindVertexArray(0);
}