#include "core/hepch.h"

#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"

#include "components/ComponentTransform.h"
#include "components/ComponentCamera.h"

#include "ui/WindowScene.h"

Hachiko::ModuleCamera::ModuleCamera() = default;

Hachiko::ModuleCamera::~ModuleCamera() = default;

bool Hachiko::ModuleCamera::Init()
{
    HE_LOG("Creating main camera");

    editor_camera_game_object = new GameObject("Main Camera");
    editor_camera = static_cast<ComponentCamera*>(editor_camera_game_object->CreateComponent(Component::Type::CAMERA));

    editor_camera_game_object->GetTransform()->SetPosition(float3(0.0f, 8.0f, 10.0f));
    editor_camera_game_object->GetTransform()->LookAtTarget(float3::zero);
    editor_camera_game_object->Update();

    ImGuizmo::Enable(true);

    // TODO: LOOK FOR THE MAIN CAMERA

    return true;
}

UpdateStatus Hachiko::ModuleCamera::Update(const float delta)
{
    if (editor_camera_enable)
    {
        Controller(delta);
    }
    return UpdateStatus::UPDATE_CONTINUE;
}

bool Hachiko::ModuleCamera::CleanUp()
{
    delete editor_camera_game_object;

    return true;
}

void Hachiko::ModuleCamera::OnResize(unsigned int screen_width, unsigned int screen_height) const
{
    editor_camera->SetResolution(screen_width, screen_height);
    if (main_camera != nullptr)
    {
        main_camera->SetResolution(screen_width, screen_height);
    }
}

void Hachiko::ModuleCamera::Controller(const float delta) const
{
    static const float zoom_speed = 3.0f;
    static const float rot_speed = 2.0f;
    static const float perpendicular_movement_speed = 2.0f;

#ifndef PLAY_BUILD
    if (!App->editor->GetSceneWindow()->IsHovering())
        return;
#endif

    // Keyboard movement ---------------
    if (App->input->GetMouseButton(SDL_BUTTON_RIGHT))
    {
        int moved_x, moved_y;
        App->input->GetMouseDelta(moved_x, moved_y);

        Rotate(-static_cast<float>(moved_x) * delta * rot_speed, static_cast<float>(moved_y) * delta * rot_speed);
        MovementController(delta);
    }

    // Mouse ----------------------------
    const int scrolled_y = App->input->GetScrollDelta();
    if (scrolled_y != 0)
        Zoom(static_cast<float>(scrolled_y) * zoom_speed);

    if (App->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_DOWN)
    {
        int moved_x, moved_y;
        App->input->GetMouseDelta(moved_x, moved_y);
        Orbit(moved_x * 1.5f, moved_y * 1.5f);
    }
    if (App->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_DOWN)
    {
        const float distance = (editor_camera->reference_point - editor_camera->GetGameObject()->GetTransform()->GetPosition()).Length();
        GameObject* go = App->editor->GetSelectedGameObject();
        FocusOnModel(go->GetTransform()->GetPosition(), distance);
    }
    if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE))
    {
        int moved_x, moved_y;
        App->input->GetMouseDelta(moved_x, moved_y);

        PerpendicularMovement((float)moved_x * delta * perpendicular_movement_speed, (float)moved_y * delta * perpendicular_movement_speed);
    }
}

void Hachiko::ModuleCamera::Zoom(float zoom) const
{
    auto* transform = editor_camera->GetGameObject()->GetTransform();
    const float distance = editor_camera->reference_point.Distance(transform->GetPosition());
    const vec newPos = transform->GetPosition() + zoom * transform->GetFront() * distance * 0.05f;

    transform->SetPosition(newPos);
    editor_camera->GetGameObject()->Update();
}

void Hachiko::ModuleCamera::Orbit(float motion_x, float motion_y) const
{
    auto* transform = editor_camera->GetGameObject()->GetTransform();
    float3 vector = transform->GetPosition() - editor_camera->reference_point;

    vector = Quat(transform->GetUp(), motion_x * 0.003f).Transform(vector);
    vector = Quat(transform->GetRight().Neg(), motion_y * 0.003f).Transform(vector);

    transform->SetPosition(vector + editor_camera->reference_point);

    transform->LookAtTarget(editor_camera->reference_point);
    editor_camera->GetGameObject()->Update();
}

void Hachiko::ModuleCamera::MovementController(const float delta) const
{
    static const float move_speed = 15.0f;
    static const float speed_modifier = 2.0f;

    float effective_speed = move_speed;
    if (App->input->GetKeyMod(KMOD_SHIFT))
        effective_speed *= speed_modifier;

    auto* transform = editor_camera->GetGameObject()->GetTransform();
    float3 deltaRight = float3::zero, deltaUp = float3::zero, deltaFwd = float3::zero;

    if (App->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
        deltaFwd += transform->GetFront() * delta * effective_speed;
    if (App->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
        deltaFwd -= transform->GetFront() * delta * effective_speed;
    if (App->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
        deltaRight += transform->GetRight() * delta * effective_speed;
    if (App->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
        deltaRight -= transform->GetRight() * delta * effective_speed;
    if (App->input->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_REPEAT)
        deltaUp += float3::unitY * delta * effective_speed;
    if (App->input->GetKey(SDL_SCANCODE_E) == KeyState::KEY_REPEAT)
        deltaUp -= float3::unitY * delta * effective_speed;

    transform->SetPosition(transform->GetPosition() + deltaFwd + deltaRight + deltaUp);
    editor_camera->GetGameObject()->Update();
    editor_camera->reference_point += deltaRight + deltaUp;
}

void Hachiko::ModuleCamera::FocusOnModel(const float3& target, float distance) const
{
    auto* transform = editor_camera->GetGameObject()->GetTransform();
    const float3 v = transform->GetFront().Neg();

    transform->SetPosition(target + (v * distance));
    editor_camera->GetGameObject()->Update();

    editor_camera->reference_point = target;
}

void Hachiko::ModuleCamera::Rotate(float motion_x, float motion_y) const
{
    auto* transform = editor_camera_game_object->GetTransform();

    const Quat yaw_quat = Quat::RotateY(motion_x);
    const float3 newRight = yaw_quat * transform->GetRight();

    const Quat pitch_quat = Quat::RotateAxisAngle(newRight, motion_y);
    const float3 newUp = pitch_quat * yaw_quat * transform->GetUp();
    const float3 newFwd = pitch_quat * yaw_quat * transform->GetFront();

    transform->SetRotationAxis(newRight, newUp, newFwd);
    editor_camera->GetGameObject()->Update();

    const float distancetoReference = (editor_camera->reference_point - transform->GetPosition()).Length();
    editor_camera->reference_point = transform->GetPosition() + newFwd * distancetoReference;
}

void Hachiko::ModuleCamera::PerpendicularMovement(float motion_x, float motion_y) const
{
    static const float move_speed = 15.0f;

    ComponentTransform* transform = editor_camera->GetGameObject()->GetTransform();
    float3 deltaMovement = transform->GetRight() * move_speed * motion_x + transform->GetUp() * move_speed * motion_y;

    transform->SetPosition(transform->GetPosition() + deltaMovement);
    editor_camera->GetGameObject()->Update();
    editor_camera->reference_point += deltaMovement;
}