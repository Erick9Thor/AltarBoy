#include "core/hepch.h"

#include "ComponentVideo.h"

#include "modules/ModuleWindow.h"

Hachiko::ComponentVideo::ComponentVideo(GameObject* container)
: Component(Component::Type::VIDEO, container)
{
}

void Hachiko::ComponentVideo::DrawGui() 
{
    if (ImGui::Button("Play"))
    {
        Play();
    }
}

void Hachiko::ComponentVideo::Save(YAML::Node& node) const {}

void Hachiko::ComponentVideo::Load(const YAML::Node& node) {}

void Hachiko::ComponentVideo::Play()
{
    cv::VideoCapture cap("C:\\Users\\SoppelsA\\Videos\\CuidaLosChanchos.mp4");
    // cap is the object of class video capture that tries to capture Bumpy.mp4
    if (!cap.isOpened())
    {
        return;
    }

    double fps = cap.get(cv::CAP_PROP_FPS); //get the frames per seconds of the video
    // The function get is used to derive a property from the element.
    // Example:
    // CV_CAP_PROP_POS_MSEC :  Current Video capture timestamp.
    // CV_CAP_PROP_POS_FRAMES : Index of the next frame.

    cv::namedWindow("A_good_name", cv::WINDOW_AUTOSIZE); //create a window called "MyVideo"
    // first argument: name of the window.
    // second argument: flag- types:
    // WINDOW_NORMAL : The user can resize the window.
    // WINDOW_AUTOSIZE : The window size is automatically adjusted to fitvthe displayed image() ), and you cannot change the window size manually.
    // WINDOW_OPENGL : The window will be created with OpenGL support.

    while (1)
    {
        const auto& window = App->window;
        cv::Mat frame();
        // Mat object is a basic image container. frame is an object of Mat.

        if (!cap.read(frame)) // if not success, break loop
        // read() decodes and captures the next frame.
        {
            break;
        }

        imshow("A_good_name", frame);
        // first argument: name of the window.
        // second argument: image to be shown(Mat object).

        if (cv::waitKey(30) == 27) // Wait for 'esc' key press to exit
        {
            break;
        }
    }

    return;
}

void Hachiko::ComponentVideo::Pause() {}

void Hachiko::ComponentVideo::Stop() {}
