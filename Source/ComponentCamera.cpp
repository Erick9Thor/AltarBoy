#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleWindow.h"

#include "ComponentTransform.h"
#include "ModuleRender.h"
#include <glew.h>

ComponentCamera::ComponentCamera(GameObject* container): Component(Component::Type::Camera, container)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 1000.0f);

	horizontal_fov = 90.0f;
	frustum.SetHorizontalFovAndAspectRatio(horizontal_fov * to_rad, (float)DEFAULT_CAMERA_WIDTH / (float)DEFAULT_CAMERA_HEIGHT);

	frustum.SetPos(float3(0.0f, 0.0f, 0.0f));
	frustum.SetFront(float3x3::identity.WorldZ());
	frustum.SetUp(float3x3::identity.WorldY());

	frustum.GetPlanes(planes);
	GenerateFrameBuffer();
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::GenerateFrameBuffer()
{
	glGenFramebuffers(1, &frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	
	glGenTextures(1, &fb_texture);
	glBindTexture(GL_TEXTURE_2D, fb_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Depth and stencil buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_texture, 0);
	glGenRenderbuffers(1, &depth_stencil_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_buffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("[M_Render] Error creating frame buffer");
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ComponentCamera::SetNearPlane(float distance)
{
	frustum.SetViewPlaneDistances(distance, frustum.FarPlaneDistance());
	frustum.GetPlanes(planes);
}

void ComponentCamera::SetFarPlane(float distance)
{
	frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), distance);
	frustum.GetPlanes(planes);
}

void ComponentCamera::SetFOV(float fov)
{
	horizontal_fov = fov;
	frustum.SetHorizontalFovAndAspectRatio(horizontal_fov * to_rad, frustum.AspectRatio());
	frustum.GetPlanes(planes);
}

float4x4 ComponentCamera::GetViewMatrix(const bool transpose) const
{
	static float4x4 view;
	view = frustum.ViewMatrix();
	if (transpose) {
		view.Transpose();
	}
	return view;
}

float4x4 ComponentCamera::GetProjectionMatrix(const bool transpose) const
{
	static float4x4 proj;
	proj = frustum.ProjectionMatrix();
	if (transpose) {
		proj.Transpose();
	}
	return proj;
}

void ComponentCamera::OnTransformUpdated()
{
	ComponentTransform* transform = game_object->GetComponent<ComponentTransform>();
	
	frustum.SetFrame(transform->GetPosition(), transform->GetFwd(), transform->GetUp());
	frustum.GetPlanes(planes);
}

void ComponentCamera::SetResolution(float width, float height)
{
	if (resolution_x != width || resolution_y != height)
	{
		resolution_x = width;
		resolution_y = height;
		ResizeFrameBuffer();
	}
	float vertical_fov = frustum.VerticalFov();
	frustum.SetVerticalFovAndAspectRatio(vertical_fov, width / height);
}

void ComponentCamera::ResizeFrameBuffer() {
	glBindTexture(GL_TEXTURE_2D, fb_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int)resolution_x, (int)resolution_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (int)resolution_x, (int)resolution_y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void ComponentCamera::DrawGui()
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}