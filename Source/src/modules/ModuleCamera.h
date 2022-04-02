#pragma once
#include "Module.h"
#include "Globals.h"

#include "core/GameObject.h"

namespace Hachiko
{
    class ComponentCamera;

    class ModuleCamera : public Module
    {
    public:
        ModuleCamera();
        ~ModuleCamera() override;

        bool Init() override;
        UpdateStatus Update(float delta) override;
        bool CleanUp() override;

        void OnResize(unsigned int screen_width, unsigned int screen_height) const;

        void Controller(float delta) const;
        void MovementController(float delta) const;

        [[nodiscard]] ComponentCamera* GetMainCamera() const
        {
            if (editor_camera_enable || main_camera == nullptr)
            {
                return editor_camera;
            }
            return main_camera;
        }
        void SetMainCamera(ComponentCamera* camera) 
        {
            if (camera != nullptr)
            {
                main_camera = camera;
                editor_camera_enable = false;
            }
        }
        void ActivateEditorCamera(bool activate) 
        {
            editor_camera_enable = activate;
        }

        // Camera actions
        void Zoom(float zoom) const;
        void Orbit(float motion_x, float motion_y) const;
        void FocusOnModel(const float3& target, float distance) const;
        void Rotate(float motion_x, float motion_y) const;
        void PerpendicularMovement(float motion_x, float motion_y) const;

    private:
        bool editor_camera_enable = true;
        GameObject* editor_camera_game_object = nullptr;
        ComponentCamera* editor_camera = nullptr;

        ComponentCamera* main_camera = nullptr;
    };
}
