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
            return main_camera;
        }
        void AddCameraComponent(Component* camera);
        void RemoveCameraComponent(Component* camera);
        void SetMainCamera(Component* camera);
        void RestoreOriginCamera();

        // Camera actions
        void Zoom(float zoom) const;
        void Orbit(float motion_x, float motion_y) const;
        void FocusOnModel(const float3& target, float distance) const;
        void Rotate(float motion_x, float motion_y) const;
        void PerpendicularMovement(float motion_x, float motion_y) const;

    private:
        std::vector<Component*> camera_buffer;
        GameObject* main_camera_game_object = nullptr; // TODO: This name may be too long for Eric or Bernat's taste.
        ComponentCamera* main_camera = nullptr;
        unsigned int last_it = 0;
    };
}
