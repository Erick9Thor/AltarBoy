#pragma once

#include "Component.h"

namespace Hachiko
{
    class ResourceVideo;

    enum class VideoState
    {
        PLAYING = 0,
        PAUSED,
        STOPPED
    };

    class ComponentVideo : public Component
    {
    public:
        ComponentVideo(GameObject* container);
        ~ComponentVideo() override;

        void Start() override;
        void Update() override;
        void DrawGui() override;
        void Draw(ComponentCamera* camera, Program* program);

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;

        HACHIKO_API void Play();
        HACHIKO_API void Pause();
        HACHIKO_API void Stop();
        HACHIKO_API void Restart();

    private:
        bool in_scene = false;
        bool projected = false;
        bool fullscreen = false;
        unsigned int frame_texture = 0;
        float timestamp = 0.0f;

        cv::VideoCapture video_capture;
        cv::Mat frame; // Basic image container.
        VideoState state = VideoState::PAUSED;
        ResourceVideo* video = nullptr;

        void DisplayControls();
        void PublishIntoScene();
        void DetachFromScene();
        void BindCVMat2GLTexture(cv::Mat& frame);
        bool IsPlaying();
        void SetProjectionMatrices(const ComponentCamera* camera, const Program* program);
        void AddVideo();
        void RemoveVideo();
        void CaptureVideo();
    };
} // namespace Hachiko