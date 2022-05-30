#include "core/hepch.h"

#include "Particle.h"

#include <imgui_color_gradient.h>

#include "modules/ModuleProgram.h"
#include "modules/ModuleResources.h"
#include "modules/ModuleRender.h"

#include "modules/ModuleCamera.h" // TODO: This is only for debug

#include "resources/ResourceTexture.h"

#include "components/ComponentTransform.h"
#include "components/ComponentCamera.h"

Hachiko::Particle::Particle()
{
    gradient = new ImGradient();
}

Hachiko::Particle::~Particle()
{
    delete gradient;
}

void Hachiko::Particle::Draw(ComponentCamera* camera, Program* program)
{
    Program* billboard_program = App->program->GetBillboardProgram(); //TODO: This must be taken from the parameter
    billboard_program->Activate();

    glActiveTexture(GL_TEXTURE0);
    int glTexture = 0;
    has_texture = 0;
    if (texture != nullptr)
    {
        glTexture = texture->GetImageId();
        Hachiko::ModuleTexture::Bind(glTexture, static_cast<int>(Hachiko::ModuleProgram::TextureSlots::DIFFUSE));
        has_texture = 1;
    }

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    if (render_mode == ParticleRenderMode::PARTICLE_ADDITIVE)
    {
        glBlendFunc(GL_ONE, GL_ONE);
    }
    else
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    float4x4 model_matrix;
    GetOrientationMatrix(camera, model_matrix);
    billboard_program->BindUniformFloat4x4("model", model_matrix.ptr());
    billboard_program->BindUniformFloat4x4("view", &camera->GetViewMatrix()[0][0]);
    billboard_program->BindUniformFloat4x4("proj", &camera->GetProjectionMatrix()[0][0]);

    billboard_program->BindUniformFloat("x_factor", &x_factor);
    billboard_program->BindUniformFloat("y_factor", &y_factor);
    billboard_program->BindUniformFloat("current_frame", &current_frame);
    billboard_program->BindUniformFloat2("animation_index", animation_index.ptr());
    float4 color = float4::one;
    if (has_color_gradient)
    {
        gradient->getColorAt(color_frame, color.ptr());
    }

    billboard_program->BindUniformFloat4("input_color", color.ptr());
    billboard_program->BindUniformInts("has_texture", 1, &has_texture);

    int flip_x = has_flip_x ? 1 : 0;
    int flip_y = has_flip_y ? 1 : 0;
    billboard_program->BindUniformInts("flip_x", 1, &flip_x);
    billboard_program->BindUniformInts("flip_y", 1, &flip_y);

    glBindVertexArray(App->renderer->billboard_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Clear
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    Program::Deactivate();
    return;
}

void Hachiko::Particle::DrawGui()
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
        ImGui::DragInt("Skip Frames", &skip_frames, 1.0f, 0, inf);
        ImGui::Checkbox("Play On Awake", &play_on_awake);

        const char* billboardTypeCombo[] = {"LookAt", "Stretch", "Horitzontal", "Vertical"};
        const char* billboardTypeComboCurrent = billboardTypeCombo[(int)type];
        ImGui::Text("Type:");
        if (ImGui::BeginCombo("##Type", billboardTypeComboCurrent))
        {
            for (int n = 0; n < IM_ARRAYSIZE(billboardTypeCombo); ++n)
            {
                bool isSelected = (billboardTypeComboCurrent == billboardTypeCombo[n]);
                if (ImGui::Selectable(billboardTypeCombo[n], isSelected))
                {
                    type = (ParticleOrientationType)n;
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if (type == ParticleOrientationType::HORIZONTAL)
        {
            ImGui::Indent();
            ImGui::Checkbox("Orientate to direction", &is_horizontal);
            ImGui::Unindent();
        }

        const char* renderModeCombo[] = {"Additive", "Transparent"};
        const char* renderModeComboCurrent = renderModeCombo[(int)render_mode];
        if (ImGui::BeginCombo("Render Mode##", renderModeComboCurrent))
        {
            for (int n = 0; n < IM_ARRAYSIZE(renderModeCombo); ++n)
            {
                bool isSelected = (renderModeComboCurrent == renderModeCombo[n]);
                if (ImGui::Selectable(renderModeCombo[n], isSelected))
                {
                    render_mode = (ParticleRenderMode)n;
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::NewLine();

        // Texture Sheet Animation
        if (ImGui::CollapsingHeader("Texture Sheet Animation"))
        {
            ImGui::Checkbox("Loop##animation_loop", &animation_loop);
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
                    if (ImGui::Button("Remove Texture##remove_texture"))
                    {
                        RemoveTexture();
                    }

                    ImGui::EndGroup();
                }
                else
                {
                    AddTexture();
                }
                ImGui::TreePop();
            }
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

            //ImGui::DragInt("Cycles##animation_cycles", &animation_cycles, 1, 1, inf);
            ImGui::Checkbox("FlipX##animation_loop", &has_flip_x);
            ImGui::SameLine();
            ImGui::Checkbox("FlipY##animation_loop", &has_flip_y);
        }

        // Color Over Lifetime
        if (ImGui::CollapsingHeader("Color over lifetime"))
        {
            ImGui::Checkbox("Activate Gradient##color_over_lifetime", &has_color_gradient);
            if (has_color_gradient)
            {
                ImGui::Checkbox("Loop##color_loop", &color_loop);
                ImGui::PushItemWidth(200);
                ImGui::GradientEditor(gradient, draggingGradient, selectedGradient);
                ImGui::PushItemWidth(150);
                ImGui::NewLine();
                ImGui::DragInt("Cycles over lifetime##color_cycles", &color_cycles, 1, 1, inf);
            }
        }
    }
}

void Hachiko::Particle::Start()
{
    is_playing = play_on_awake;
}

void Hachiko::Particle::Update()
{
    time += EngineTimer::delta_time;
    if (frame_counter++ < skip_frames)
    {
        return;
    }

    if (!is_playing)
    {
        Reset();
        return;
    }

    if (time > billboard_lifetime)
    {
        play_animation = false;
        play_color_gradient = false;
    }

    if (animation_loop || play_animation)
    {
        UpdateAnimationData();
    }

    if (color_loop || play_color_gradient)
    {
        UpdateColorOverLifetime();
    }

    frame_counter = 0;
}

inline void Hachiko::Particle::Play()
{
    Reset();
    is_playing = true;
    play_animation = true;
    play_color_gradient = true;
}

inline void Hachiko::Particle::Stop()
{
    is_playing = false;
    play_animation = false;
    play_color_gradient = false;
}

inline void Hachiko::Particle::Reset()
{
    is_playing = false;
    time = 0.0f;
    color_frame = 1.0f;
    play_animation = false;
    play_color_gradient = false;
}

void Hachiko::Particle::Save(YAML::Node& node) const
{
    node[BILLBOARD_TYPE] = static_cast<int>(type);
    node[BILLBOARD_PLAY_ON_AWAKE] = play_on_awake;
    if (texture != nullptr)
    {
        node[BILLBOARD_TEXTURE_ID] = texture->GetID();
    }
    node[X_TILES] = x_tiles;
    node[Y_TILES] = y_tiles;
    node[FLIP_X] = has_flip_x;
    node[FLIP_Y] = has_flip_y;
    node[BILLBOARD_LIFETIME] = billboard_lifetime;
    node[SKIP_FRAMES] = skip_frames;
    node[ANIMATION_LOOP] = animation_loop;
    node[HAS_COLOR_GRADIENT] = has_color_gradient;
    node[COLOR_CYCLES] = color_cycles;
    node[COLOR_LOOP] = color_loop;
    node[COLOR_GRADIENT] = *gradient;
}

void Hachiko::Particle::Load(const YAML::Node& node)
{
    UID texture_id = node[BILLBOARD_TEXTURE_ID].IsDefined() ? node[BILLBOARD_TEXTURE_ID].as<UID>() : 0;
    if (texture_id)
    {
        texture = static_cast<ResourceTexture*>(App->resources->GetResource(Resource::Type::TEXTURE, texture_id));
        has_texture = 1;
    }

    type = node[BILLBOARD_TYPE].IsDefined() ? static_cast<ParticleOrientationType>(node[BILLBOARD_TYPE].as<int>()) : ParticleOrientationType::HORIZONTAL;

    play_on_awake = node[BILLBOARD_PLAY_ON_AWAKE].IsDefined() ? node[BILLBOARD_PLAY_ON_AWAKE].as<bool>() : false;

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

    billboard_lifetime = node[BILLBOARD_LIFETIME].IsDefined() ? node[BILLBOARD_LIFETIME].as<float>() : 0.0f;

    skip_frames = node[SKIP_FRAMES].IsDefined() ? node[SKIP_FRAMES].as<int>() : 0;

    animation_loop = node[ANIMATION_LOOP].IsDefined() ? node[ANIMATION_LOOP].as<bool>() : false;

    has_flip_x = node[FLIP_X].IsDefined() ? node[FLIP_X].as<bool>() : false;

    has_flip_y = node[FLIP_Y].IsDefined() ? node[FLIP_Y].as<bool>() : false;

    has_color_gradient = node[HAS_COLOR_GRADIENT].IsDefined() ? node[HAS_COLOR_GRADIENT].as<bool>() : false;

    color_cycles = node[COLOR_CYCLES].IsDefined() ? node[COLOR_CYCLES].as<int>() : 0;

    color_loop = node[COLOR_LOOP].IsDefined() ? node[COLOR_LOOP].as<bool>() : false;

    if (node[COLOR_GRADIENT].IsDefined())
    {
        gradient->clearMarks();
        for (int i = 0; i < node[COLOR_GRADIENT].size(); ++i)
        {
            auto mark = node[COLOR_GRADIENT][i].as<ImGradientMark>();
            gradient->addMark(mark.position, ImColor(mark.color[0], mark.color[1], mark.color[2], mark.color[3]));
        }
    }
}

void Hachiko::Particle::AddTexture()
{
    const std::string title = "Select billboard texture ";
    std::string texture_path;
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
            texture_path = ImGuiFileDialog::Instance()->GetFilePathName();
        }

        ImGuiFileDialog::Instance()->Close();
    }

    texture_path.append(META_EXTENSION);
    if (!std::filesystem::exists(texture_path.c_str()))
    {
        return;
    }

    YAML::Node texture_node = YAML::LoadFile(texture_path);
    res = static_cast<ResourceTexture*>(App->resources->GetResource(Resource::Type::TEXTURE, texture_node[RESOURCES][0][RESOURCE_ID].as<UID>()));

    if (res != nullptr)
    {
        texture = res;
        textureID = texture->GetID();
        has_texture = 1;
    }
}

void Hachiko::Particle::RemoveTexture()
{
    texture = nullptr;
    textureID = 0;
}

inline void Hachiko::Particle::UpdateAnimationData()
{
    if (!has_texture)
    {
        return;
    }

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

inline void Hachiko::Particle::UpdateColorOverLifetime()
{
    float timeMod = fmod(time, billboard_lifetime / color_cycles);
    color_frame = timeMod / billboard_lifetime * color_cycles;

    if (color_loop && time > billboard_lifetime)
    {
        time = 0.0f;
        color_frame = 0.0f;
    }
}

void Hachiko::Particle::GetOrientationMatrix(ComponentCamera* camera, float4x4& model_matrix)
{
    // TODO: set go transform
    ComponentTransform* transform = nullptr; //GetGameObject()->GetComponent<ComponentTransform>();

    Frustum* frustum = camera->GetFrustum();
    float4x4* proj = &camera->GetProjectionMatrix();
    float4x4* view = &camera->GetViewMatrix();

    float3x3 rotatePart = transform->GetGlobalMatrix().RotatePart();
    float3 position = transform->GetGlobalPosition();
    float4x4 modelMatrix = transform->GetGlobalMatrix();
    float3 scale = transform->GetGlobalScale();

    if (type == ParticleOrientationType::NORMAL)
    {
        model_matrix = modelMatrix.LookAt(rotatePart.Col(2), -frustum->Front(), rotatePart.Col(1), float3::unitY);
        model_matrix = float4x4::FromTRS(position, model_matrix.RotatePart() * rotatePart, scale);
    }
    else if (type == ParticleOrientationType::STRETCH)
    {
        float3 cameraPos = camera->GetFrustum()->Pos();
        float3 cameraDir = (cameraPos - position).Normalized();
        float3 upDir = Cross(direction, cameraDir);
        float3 newCameraDir = Cross(direction, upDir);

        float3x3 newRotation;
        newRotation.SetCol(0, upDir);
        newRotation.SetCol(1, direction);
        newRotation.SetCol(2, newCameraDir);

        model_matrix = float4x4::FromTRS(position, newRotation * model_stretch, scale);
    }
    else if (type == ParticleOrientationType::HORIZONTAL)
    {
        if (is_horizontal)
        {
            float3 direction = transform->GetGlobalRotation().WorldZ();
            float3 projection = position + direction - direction.y * float3::unitY;
            direction = (projection - position).Normalized();
            float3 right = Cross(float3::unitY, direction);

            float3x3 newRotation;
            newRotation.SetCol(1, right);
            newRotation.SetCol(2, float3::unitY);
            newRotation.SetCol(0, direction);

            model_matrix = float4x4::FromTRS(position, newRotation, scale);
        }
        else
        {
            model_matrix = float4x4::LookAt(float3::unitZ, float3::unitY, float3::unitY, float3::unitY);
            model_matrix = float4x4::FromTRS(position, model_matrix.RotatePart(), scale);
        }
    }
    else if (type == ParticleOrientationType::VERTICAL)
    {
        float3 cameraPos = camera->GetFrustum()->Pos();
        float3 cameraDir = (float3(cameraPos.x, position.y, cameraPos.z) - position).Normalized();
        model_matrix = float4x4::LookAt(float3::unitZ, cameraDir, float3::unitY, float3::unitY);
        model_matrix = float4x4::FromTRS(position, model_matrix.RotatePart(), scale);
    }
}
