#pragma once
#include "Window.h"
#include <imgui.h>
#include <ImGuizmo.h>

#include "utils/Timer.h"

namespace Hachiko
{
    class GameObject;
    class ComponentCamera;
    class Scene;

    class WindowScene final : public Window
    {
    public:
        WindowScene();
        ~WindowScene() override;
        void Update() override;

        void CleanUp() override;

        [[nodiscard]] bool IsHovering() const
        {
            return hovering;
        }

    private:
        void GuizmoOptionsController();
        void ToolbarMenu() const;
        void DrawScene();
        void Controller() const;

        GameObject* SelectObject(ComponentCamera* camera, Scene* scene) const;

        ImVec2 imguizmo_size = {100.0f, 100.0f};
        bool using_guizmo = false;
        bool focused = false;
        bool hovering = false;

        Timer* scene_timer = nullptr;

        ImGuizmo::OPERATION guizmo_operation = ImGuizmo::TRANSLATE;
        ImGuizmo::MODE guizmo_mode = ImGuizmo::WORLD;

        ImVec2 guizmo_rect_origin = {0.0f, 0.0f};
        float2 texture_position = float2(0.0f);
        float2 texture_size = {0.0f, 0.0f};
    };
}
