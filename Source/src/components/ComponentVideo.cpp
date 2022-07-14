#include "core/hepch.h"

#include "ComponentVideo.h"

#include "modules/ModuleWindow.h"
#include "modules/ModuleProgram.h"
#include "modules/ModuleResources.h"

#include "ComponentCamera.h"
#include "ComponentTransform.h"

#include "resources/ResourceVideo.h"

Hachiko::ComponentVideo::ComponentVideo(GameObject* container)
: Component(Component::Type::VIDEO, container)
{}

Hachiko::ComponentVideo::~ComponentVideo()
{
    glDeleteTextures(1, &frame_texture);
    DetachFromScene();
    video_capture.release();
    RemoveVideo();
}

void Hachiko::ComponentVideo::Start()
{
    if (!in_scene)
    {
        PublishIntoScene();
    }

    glGenTextures(1, &frame_texture);
    CaptureVideo();
}

void Hachiko::ComponentVideo::Restart()
{
    video_capture.set(cv::CAP_PROP_POS_FRAMES, 0.0f);
    timestamp = 0.0f;
    state = VideoState::PLAYING;
}

void Hachiko::ComponentVideo::Update()
{
#ifndef PLAY_BUILD
    if (!in_scene)
    {
        Start();
    }
#endif //PLAY_BUILD

    if (IsPlaying())
    {
        timestamp += EngineTimer::delta_time * 1000;
        video_capture.set(cv::CAP_PROP_POS_MSEC, timestamp);
    }
}

void Hachiko::ComponentVideo::DrawGui()
{
    ImGui::PushID(this);
    if (ImGuiUtils::CollapsingHeader(game_object, this, "Video"))
    {
        if (video == nullptr)
        {
            AddVideo();
        }
        else if (ImGui::Button("Remove video", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
        {
            RemoveVideo();
        }

        Widgets::Checkbox("Projected", &projected);
        Widgets::Checkbox("Fullsceen", &fullscreen);
    }
    ImGui::PopID();

#ifndef PLAY_BUILD
    if (video != nullptr)
    {
        DisplayControls();
    }
#endif
}

void Hachiko::ComponentVideo::Draw(ComponentCamera* camera, Program* /*program*/)
{
    if (state == VideoState::STOPPED || video == nullptr)
    {
        return;
    }
    
    const Hachiko::Program* program = App->program->GetVideoProgram();
    program->Activate();
    glDepthMask(GL_FALSE);

    if (IsPlaying())
    {
        // read() decodes and captures the next frame.
        if (video_capture.read(frame))
        {
            cv::flip(frame, frame, 0);
        }
        else
        {
            Stop();
        }
    }

    BindCVMat2GLTexture(frame);
    SetProjectionMatrices(camera, program);

    glBindVertexArray(App->renderer->GetVideoVao());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Clear
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDepthMask(GL_TRUE);
    program->Deactivate();
}

void Hachiko::ComponentVideo::Save(YAML::Node& node) const
{
    node.SetTag("video");
    node[VIDEO_ID] = video != nullptr ? video->GetID() : 0;
    node[VIDEO_PROJECTED] = projected;
    node[VIDEO_FULLSCREEN] = fullscreen;

}

void Hachiko::ComponentVideo::Load(const YAML::Node& node)
{
    if (node[VIDEO_ID].IsDefined() && node[VIDEO_ID].as<UID>() != 0)
    {
        video = static_cast<ResourceVideo*>(App->resources->GetResource(Resource::Type::VIDEO, node[VIDEO_ID].as<UID>()));
    }

    projected = node[VIDEO_PROJECTED].IsDefined() ? node[VIDEO_PROJECTED].as<bool>() : projected;
    fullscreen = node[VIDEO_FULLSCREEN].IsDefined() ? node[VIDEO_FULLSCREEN].as<bool>() : fullscreen;
}

void Hachiko::ComponentVideo::Play()
{
    state = VideoState::PLAYING;
}

void Hachiko::ComponentVideo::Pause()
{
    state = VideoState::PAUSED;
}

void Hachiko::ComponentVideo::Stop()
{
    Restart();
    state = VideoState::STOPPED;
}

void Hachiko::ComponentVideo::BindCVMat2GLTexture(cv::Mat& frame)
{
    if (frame.empty())
    {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, frame_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set texture clamping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, // Type of texture
                 0, // Pyramid level (for mip-mapping) - 0 is the top level
                 GL_RGB, // Internal colour format to convert to
                 frame.cols, // Image width  i.e. 640 for Kinect in standard mode
                 frame.rows, // Image height i.e. 480 for Kinect in standard mode
                 0, // Border width in pixels (can either be 1 or 0)
                 GL_BGR, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                 GL_UNSIGNED_BYTE, // Image data type
                 frame.ptr()); // The actual image data itself

    glGenerateMipmap(GL_TEXTURE_2D);
}

bool Hachiko::ComponentVideo::IsPlaying()
{
    return state == VideoState::PLAYING;
}

void Hachiko::ComponentVideo::SetProjectionMatrices(const ComponentCamera* camera, const Program* program)
{
    ComponentTransform* transform = GetGameObject()->GetComponent<ComponentTransform>();
    float3 scale = transform->GetGlobalScale();

    if (fullscreen)
    {
        scale.x = App->window->GetWidth() / (float)frame.cols;
        scale.y = App->window->GetHeight() / (float)frame.rows;
    }

    float4x4 model_matrix = float4x4::FromTRS(transform->GetGlobalPosition(), transform->GetGlobalRotation(), scale);

    program->BindUniformInt("ignore_camera", projected ? 0 : 1);
    program->BindUniformFloat4x4("model", model_matrix.ptr());
    program->BindUniformFloat4x4("view", &camera->GetViewMatrix()[0][0]);
    program->BindUniformFloat4x4("proj", &camera->GetProjectionMatrix()[0][0]);
}

void Hachiko::ComponentVideo::AddVideo()
{
    const char* title = "Select video";
    std::string video_path;

    if (ImGui::Button("Add video", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
    {
        ImGuiFileDialog::Instance()->OpenDialog(title, "Select Video", ".mp4", "./assets/video/", 1, nullptr,
           ImGuiFileDialogFlags_DontShowHiddenFiles | ImGuiFileDialogFlags_DisableCreateDirectoryButton | 
           ImGuiFileDialogFlags_HideColumnType | ImGuiFileDialogFlags_HideColumnDate);
    }

    if (ImGuiFileDialog::Instance()->Display(title))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            video_path = ImGuiFileDialog::Instance()->GetFilePathName();
        }

        ImGuiFileDialog::Instance()->Close();
    }

    video_path.append(META_EXTENSION);

    if (!std::filesystem::exists(video_path.c_str()))
    {
        return;
    }

    YAML::Node video_node = YAML::LoadFile(video_path);
    ResourceVideo* res = static_cast<ResourceVideo*>(App->resources->GetResource(Resource::Type::VIDEO, video_node[RESOURCES][0][RESOURCE_ID].as<UID>()));

    if (res != nullptr)
    {
        video = res;
        CaptureVideo();
    }
}

void Hachiko::ComponentVideo::RemoveVideo()
{
    Stop();
    App->resources->ReleaseResource(video);
    video = nullptr;
}

void Hachiko::ComponentVideo::CaptureVideo()
{
    if (video == nullptr)
    {
        return;
    }
    
    video_capture = cv::VideoCapture(video->GetVideoPath().string());

    if (!video_capture.isOpened())
    {
        HE_LOG("Error while loading video capture");
        RemoveVideo();
    }
}

void Hachiko::ComponentVideo::PublishIntoScene()
{
    auto scene = game_object->scene_owner;

    if (scene)
    {
        scene->AddParticleComponent(this);
        in_scene = true;
    }
}

void Hachiko::ComponentVideo::DetachFromScene()
{
    auto scene = game_object->scene_owner;

    if (scene)
    {
        scene->RemoveParticleComponent(GetID());
        in_scene = false;
    }
}
void Hachiko::ComponentVideo::DisplayControls()
{
    const auto& pos = App->editor->GetSceneWindow()->GetViewportPosition();
    const auto& viewport_size = App->editor->GetSceneWindow()->GetViewportSize();

    ImGui::SetNextWindowPos(ImVec2(pos.x + ImGui::GetStyle().FramePadding.x * 3, pos.y));
    ImGui::SetNextWindowBgAlpha(1.0f);

    const std::string go_label = StringUtils::Concat("GameObject: ", GetGameObject()->GetName().c_str());
    const float min_size_x = std::min(ImGui::CalcTextSize(go_label.c_str()).x + ImGui::GetFontSize() * 2, viewport_size.x / 4.0f);
    ImGui::SetNextWindowSize(ImVec2(std::max(min_size_x, 200.0f), 0));
    ImGui::Begin("Video", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);

    ImGui::TextWrapped(go_label.c_str());
    ImGui::Text("Video fps: %.1f", video_capture.get(cv::CAP_PROP_FPS));
    ImGui::Text("Playing time: %.1f", video_capture.get(cv::CAP_PROP_POS_MSEC) / 1000.0);
    ImGui::Text("Frame index: %.1f", video_capture.get(cv::CAP_PROP_POS_FRAMES));

    ImGui::Separator();

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
    ImGui::BeginTable("##video", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_NoSavedSettings);
    ImGui::TableSetupColumn("##video", ImGuiTableColumnFlags_WidthStretch, 0.33f);
    ImGui::TableSetupColumn("##video", ImGuiTableColumnFlags_WidthStretch, 0.34f);
    ImGui::TableSetupColumn("##video", ImGuiTableColumnFlags_WidthStretch, 0.33f);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);

    if (state == VideoState::PLAYING)
    {
        if (ImGui::Button("Pause", ImVec2(-1, 0)))
        {
            Pause();
        }
    }
    else
    {
        if (ImGui::Button("Play", ImVec2(-1, 0)))
        {
            Play();
        }
    }
    ImGui::TableNextColumn();
    if (ImGui::Button("Restart", ImVec2(-1, 0)))
    {
        Restart();
    }
    ImGui::TableNextColumn();
    if (ImGui::Button("Stop", ImVec2(-1, 0)))
    {
        Stop();
    }
    ImGui::EndTable();
    ImGui::PopStyleVar();
    ImGui::End();
}