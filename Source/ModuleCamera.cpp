#include "ModuleCamera.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#include "glew.h"
#include "Math/MathConstants.h"

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
    position = float3(0.0f, 0.0f, 10.0f);

    frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
    auto screen_surface = App->window->screen_surface;

    SetAspectRatio(screen_surface->w, screen_surface->h);
    SetHorizontalFov(90.0f);

    RefreshFov();
    frustum.SetViewPlaneDistances(0.1f, 200.0f);

    SetPosition(position);
    frustum.SetFront(float3::unitZ);
    frustum.SetUp(float3::unitY);

    LookAt(float3(0.0f, 0.0f, 0.0f));
    return true;
}

update_status ModuleCamera::PreUpdate()
{
    SetPosition(position);

    if (locked)
        LookAt(float3(0.0f, 0.0f, 0.0f));

    return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
    return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
    ModuleCamera::checkCameraControl();
    return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
    return false;
}

void ModuleCamera::SetPosition(const float3& new_position)
{
    frustum.SetPos(position);
}

void ModuleCamera::SetAspectRatio(unsigned int screen_width, unsigned int screen_height)
{
    aspect_ratio = (float)screen_width / (float)screen_height;
}

void ModuleCamera::SetHorizontalFov(float fov_deg)
{
    static const float deg_to_rad = pi / 180.0f;
    horizontal_fov = fov_deg * deg_to_rad;
}

void ModuleCamera::LookAt(const float3& look_position)
{
    float3 direction = look_position - frustum.Pos();
    float3x3 look_rotation = float3x3::LookAt(frustum.Front(), direction.Normalized(), frustum.Up(), float3::unitY);
    frustum.SetFront(look_rotation.MulDir(frustum.Front()).Normalized());
    frustum.SetUp(look_rotation.MulDir(frustum.Up()).Normalized());
}

void ModuleCamera::RefreshFov()
{
    frustum.SetHorizontalFovAndAspectRatio(horizontal_fov, aspect_ratio);
}

void ModuleCamera::WindowResized(unsigned int screen_width, unsigned int screen_height)
{
    SetAspectRatio(screen_width, screen_height);
    RefreshFov();
}

float4x4 ModuleCamera::GetGLView() const
{
    return float4x4(frustum.ViewMatrix()).Transposed();
}

float4x4 ModuleCamera::GetView() const
{
    return float4x4(frustum.ViewMatrix());
}

float4x4 ModuleCamera::GetGLProjection() const
{
    return frustum.ProjectionMatrix().Transposed();
}

float4x4 ModuleCamera::GetProjection() const
{
    return frustum.ProjectionMatrix();
}

void ModuleCamera::checkCameraControl()
{
    bool change = false;


    if (App->input->GetKey(SDL_SCANCODE_W))
    {
        position += float3(0.0f, 0.0f, -0.1f);
        change = true;
    }
    if (App->input->GetKey(SDL_SCANCODE_S))
    {
        position += float3(0.0f, 0.0f, 0.1f);
        change = true;
    }
    if (App->input->GetKey(SDL_SCANCODE_A))
    {
        position += float3(-0.1f, 0.0f, 0.0f);
        change = true;
    }
    if (App->input->GetKey(SDL_SCANCODE_D))
    {
        position += float3(0.1f, 0.0f, 0.0f);
        change = true;
    }


    if (change) {
        frustum.SetVerticalFovAndAspectRatio(DEGTORAD * 45.0f, frustum.AspectRatio());
        frustum.SetPos(position);
    }
}