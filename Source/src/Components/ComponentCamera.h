#include "Component.h"
#include "../Utils/Math.h"

#define DEFAULT_CAMERA_WIDTH 1920
#define DEFAULT_CAMERA_HEIGHT 1080

class GameObject;

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* conatiner);
	~ComponentCamera() override;

	void Draw(ComponentCamera* camera) override;

	void GenerateFrameBuffer();
	void ResizeFrameBuffer();
	unsigned int GetFrameBuffer() const
	{
		return frame_buffer;
	}
	unsigned int GetTextureId() const
	{
		return fb_texture;
	}

	void SetNearPlane(float distance);
	void SetFarPlane(float distance);
	void SetPlaneDistances(const float near_distance, const float far_distance);
	void SetFOV(float fov);

	float4x4 GetViewMatrix(const bool transpose = false) const;
	float4x4 GetProjectionMatrix(const bool transpose = false) const;

	void OnTransformUpdated() override;

	void SetResolution(unsigned width, unsigned height);
	void GetResolution(unsigned& width, unsigned& height);

	static inline Type GetType()
	{
		return Type::Camera;
	};

	void DrawGui() override;

	Plane planes[6];
	float3 reference_point = float3::zero;
	bool draw_frustum = false;

private:
	float horizontal_fov;
	Frustum frustum;

	// FrameBuffer and depht stencil buffer
	unsigned int frame_buffer = 0;
	unsigned int fb_texture = 0;
	unsigned int depth_stencil_buffer = 0;

	unsigned resolution_x = DEFAULT_CAMERA_WIDTH;
	unsigned resolution_y = DEFAULT_CAMERA_HEIGHT;
};
