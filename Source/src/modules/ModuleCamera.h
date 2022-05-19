#pragma once
#include "Module.h"
#include "Globals.h"

#include "core/GameObject.h"

namespace Hachiko
{
    class ComponentCamera;
    class CameraPreferences;

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

        [[nodiscard]] ComponentCamera* GetRenderingCamera() 
        {
            return rendering_camera;
        }

        [[nodiscard]] ComponentCamera* GetEditorCamera()
        {
            return camera_buffer[0];
        }

        void AddCameraComponent(ComponentCamera* camera);
        void RemoveCameraComponent(ComponentCamera* camera);
        void SetRenderingCamera(ComponentCamera* camera);
        void RestoreOriginCamera();
        void ReturnPlayerCamera();

        // Camera actions
        void Zoom(float zoom) const;
        void Orbit(float motion_x, float motion_y) const;
        void FocusOnModel(const float3& target, float distance) const;
        void Rotate(float motion_x, float motion_y) const;
        void PerpendicularMovement(float motion_x, float motion_y) const;
        void RunDynamicScript(const float delta);

    private:
        
        GameObject* editor_camera_game_object = nullptr;
        ComponentCamera* rendering_camera = nullptr;
        CameraPreferences* camera_prefs = nullptr;

        std::vector<ComponentCamera*> camera_buffer;
        unsigned int last_it = 0;
    };
}
