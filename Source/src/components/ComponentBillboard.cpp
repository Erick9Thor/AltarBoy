#include "core/hepch.h"

#include "ComponentBillboard.h"

#include <imgui_color_gradient.h>

#include "modules/ModuleProgram.h"
#include "modules/ModuleResources.h"
#include "modules/ModuleRender.h"

#include "modules/ModuleCamera.h" // TODO: This is only for debug

#include "resources/ResourceTexture.h"

#include "ComponentTransform.h"
#include "ComponentCamera.h"

Hachiko::ComponentBillboard::ComponentBillboard(GameObject* container, UID id) 
	: Component(Component::Type::BILLBOARD, container, id) 
{
    gradient = new ImGradient();
}

Hachiko::ComponentBillboard::~ComponentBillboard() 
{
    delete gradient;
}

void Hachiko::ComponentBillboard::Draw(ComponentCamera* camera, Program* program)
{
    Program* billboard_program = App->program->GetBillboardProgram();
    billboard_program->Activate();

    glActiveTexture(GL_TEXTURE0);
    int glTexture = 0;
    if (texture != nullptr)
    {
        glTexture = texture->GetImageId();
        Hachiko::ModuleTexture::Bind(glTexture, static_cast<int>(Hachiko::ModuleProgram::TextureSlots::DIFFUSE));
    }
    int hasDiffuseMap = texture ? 1 : 0;

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    if (renderMode == BillboardRenderMode::B_ADDITIVE)
    {
        glBlendFunc(GL_ONE, GL_ONE);
    }
    else
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

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

    billboard_program->BindUniformFloat4x4("model", newModelMatrix.ptr());
    billboard_program->BindUniformFloat4x4("view", &camera->GetViewMatrix()[0][0]);
    billboard_program->BindUniformFloat4x4("proj", &camera->GetProjectionMatrix()[0][0]);

    billboard_program->BindUniformFloat("x_factor", &x_factor);
    billboard_program->BindUniformFloat("y_factor", &y_factor);
    billboard_program->BindUniformFloat2("animation_index", animation_index.ptr());

    float4 color = float4::one;
    if (colorOverLifetime)
    {
        gradient->getColorAt(colorFrame, color.ptr());
    }

    program->BindUniformInts("hasDiffuseMap", 1, &hasDiffuseMap);
    
    glBindVertexArray(App->renderer->billboard_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    
    // Clear
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
    Program::Deactivate();
    return;

    //program->BindUniformFloat4("inputColor", color.ptr());
    //program->BindUniformFloat3("intensity", textureIntensity.ptr());
    //program->BindUniformFloat("currentFrame", &currentFrame);
    //program->BindUniformInts("Xtiles", 1, &Xtiles);
    //program->BindUniformInts("Ytiles", 1, &Ytiles);

    //int flip_x = flipTexture[0] ? 1 : 0;
    //int flip_y = flipTexture[1] ? 1 : 0;
    //program->BindUniformInts("flipX", 1, &flip_x);
    //program->BindUniformInts("flipY", 1, &flip_y);
    //glDepthMask(GL_TRUE);
}

void Hachiko::ComponentBillboard::DrawGui()
{
    if (ImGui::CollapsingHeader("Billboard", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Separator();
        ImGui::Indent();
        ImGui::PushItemWidth(150);

        if (ImGui::Button("Play"))
        {
            Play();
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop"))
        {
            Stop();
        }
        ImGui::DragFloat("Duration", &billboard_lifetime, 1.0f, 0, inf);
        ImGui::Checkbox("Play On Awake", &playOnAwake);

        const char* billboardTypeCombo[] = {"LookAt", "Stretch", "Horitzontal", "Vertical"};
        const char* billboardTypeComboCurrent = billboardTypeCombo[(int)billboardType];
        ImGui::Text("Type:");
        if (ImGui::BeginCombo("##Type", billboardTypeComboCurrent))
        {
            for (int n = 0; n < IM_ARRAYSIZE(billboardTypeCombo); ++n)
            {
                bool isSelected = (billboardTypeComboCurrent == billboardTypeCombo[n]);
                if (ImGui::Selectable(billboardTypeCombo[n], isSelected))
                {
                    billboardType = (BillboardType)n;
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if (billboardType == BillboardType::HORIZONTAL)
        {
            ImGui::Indent();
            ImGui::Checkbox("Orientate to direction", &isHorizontalOrientation);
            ImGui::Unindent();
        }
        ImGui::NewLine();

        const char* renderModeCombo[] = {"Additive", "Transparent"};
        const char* renderModeComboCurrent = renderModeCombo[(int)renderMode];
        if (ImGui::BeginCombo("Render Mode##", renderModeComboCurrent))
        {
            for (int n = 0; n < IM_ARRAYSIZE(renderModeCombo); ++n)
            {
                bool isSelected = (renderModeComboCurrent == renderModeCombo[n]);
                if (ImGui::Selectable(renderModeCombo[n], isSelected))
                {
                    renderMode = (BillboardRenderMode)n;
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::NewLine();

        static const ImGuiTreeNodeFlags texture_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

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

      	// Texture Sheet Animation
        if (ImGui::CollapsingHeader("Texture Sheet Animation"))
        {
            if (ImGui::DragScalar("Xtiles", ImGuiDataType_U32, &x_tiles))
            {
                if (x_tiles)
                {
                    x_factor = 1 / (float)x_tiles;
                    HE_LOG("x_factor: %f", x_factor);
                }
            }
            if (ImGui::DragScalar("Ytiles", ImGuiDataType_U32, &y_tiles))
            {
                if (y_tiles)
                {
                    y_factor = 1 / (float)y_tiles;
                    HE_LOG("y_factor: %f", y_factor);
                }
            }
            ImGui::DragFloat("Cycles##animation_cycles", &animation_cycles, 1.0f, 1.0f, inf);
            ImGui::Checkbox("Loop##animation_loop", &animation_loop);
        }

        // Color Over Lifetime
        if (ImGui::CollapsingHeader("Color over lifetime"))
        {
            ImGui::Checkbox("##color_over_lifetime", &colorOverLifetime);
            if (colorOverLifetime)
            {
                ImGui::SameLine();
                ImGui::PushItemWidth(200);
                ImGui::GradientEditor(gradient, draggingGradient, selectedGradient);
                ImGui::PushItemWidth(150);
                ImGui::NewLine();
                ImGui::DragFloat("Cycles##color_cycles", &colorCycles, 1.0f, 1.0f, inf);
                ImGui::Checkbox("Loop##color_loop", &colorLoop);
            }
        }
    }
}

void Hachiko::ComponentBillboard::Update() 
{
    if (!is_playing)
    {
        return;
    }
    
    UpdateAnimationIndex();
}

void Hachiko::ComponentBillboard::Play() 
{
    is_playing = true;
}

void Hachiko::ComponentBillboard::Stop()
{
    is_playing = false;
}

void Hachiko::ComponentBillboard::Save(YAML::Node& node) const
{
    node[BILLBOARD_TYPE] = static_cast<int>(billboardType);
    if (texture != nullptr)
    {
        node[BILLBOARD_TEXTURE_ID] = texture->GetID();
    }
    node[X_TILES] = x_tiles;
    node[Y_TILES] = y_tiles;
}

void Hachiko::ComponentBillboard::Load(const YAML::Node& node) 
{
    UID texture_id = node[BILLBOARD_TEXTURE_ID].IsDefined() ? node[BILLBOARD_TEXTURE_ID].as<UID>() : 0;
    if (texture_id)
    {
        texture = static_cast<ResourceTexture*>(
            App->resources->GetResource(Resource::Type::TEXTURE, texture_id));
    }
    billboardType = node[BILLBOARD_TYPE].IsDefined() 
        ? static_cast<BillboardType>(node[BILLBOARD_TYPE].as<int>()) 
        : BillboardType::HORIZONTAL;

    x_tiles = 1;
    if (node[X_TILES].IsDefined() && !node[X_TILES].IsNull())
    {
        x_tiles = node[X_TILES].as<int>();
        x_factor = 1 / (float)x_tiles;
    }

    y_tiles = 1;
    if (node[Y_TILES].IsDefined() && !node[Y_TILES].IsNull())
    {
        y_tiles = node[Y_TILES].as<int>();
        y_factor = 1 / (float)y_tiles;
    }
}

void Hachiko::ComponentBillboard::AddTexture() 
{
    const std::string title = "Select billboard texture ";
    ResourceTexture* res = nullptr;

    if (ImGui::Button("Add Texture"))
    {
        ImGuiFileDialog::Instance()->OpenDialog(title.c_str(),
            "Select Texture", ".png,.tif,.jpg,.tga", "./assets/textures/", 1, nullptr,
            ImGuiFileDialogFlags_DontShowHiddenFiles 
            | ImGuiFileDialogFlags_DisableCreateDirectoryButton 
            | ImGuiFileDialogFlags_HideColumnType
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

void Hachiko::ComponentBillboard::UpdateAnimationIndex() 
{
    if (animation_index.x < x_tiles - 1)
    {
        animation_index.x += 1.0f;
        return;
    }
    else if (animation_index.y < y_tiles - 1)
    {
        animation_index.x = 0.0f;
        animation_index.y += 1.0f;
        return;
    }
    animation_index = {0.0f, 0.0f};
}
