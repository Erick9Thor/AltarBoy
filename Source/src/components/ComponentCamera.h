#pragma once
#include "Component.h"
#include <MathGeoLib.h>

#define DEFAULT_CAMERA_WIDTH 1920
#define DEFAULT_CAMERA_HEIGHT 1080

namespace Hachiko
{
    class GameObject;

    class ComponentCamera : public Component
    {
    public:
        ComponentCamera(GameObject* conatiner);
        ~ComponentCamera() override;

        void OnTransformUpdated() override;

        static Type GetType()
        {
            return Type::CAMERA;
        }

        [[nodiscard]] float4x4 GetViewMatrix(bool transpose = false) const;
        [[nodiscard]] float4x4 GetProjectionMatrix(bool transpose = false) const;

        Frustum* ComponentCamera::GetFrustum()
        {
            return &frustum;
        }

        void SetNearPlane(float distance);
        void SetFarPlane(float distance);
        void SetPlaneDistances(float near_distance, float far_distance);
        void SetHorizontalFov(float fov_deg);

        void SetResolution(unsigned width, unsigned height);
        void GetResolution(unsigned& width, unsigned& height) const;

        void Save(YAML::Node& node) const override;
        void Load(const YAML::Node& node) override;

        void DrawGui() override;

        void DebugDraw() override;

        LineSegment RayCast(float x, float y) const;

        Plane planes[6];
        float3 reference_point = float3::zero;
        bool draw_frustum = false;

    private:
        float horizontal_fov;
        Frustum frustum;

        unsigned resolution_x = DEFAULT_CAMERA_WIDTH;
        unsigned resolution_y = DEFAULT_CAMERA_HEIGHT;
    };
}
