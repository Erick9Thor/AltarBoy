#include "core/hepch.h"
#include "WindowScene.h"

#include "modules/ModuleInput.h"
#include "modules/ModuleEditor.h"
#include "modules/ModuleCamera.h"
#include "modules/ModuleRender.h"
#include "modules/ModuleSceneManager.h"

#include "components/ComponentCamera.h"
#include "components/ComponentTransform.h"

#include "modules/ModuleWindow.h"

Hachiko::WindowScene::WindowScene() :
    Window("Scene", true) {}

Hachiko::WindowScene::~WindowScene() = default;

void Hachiko::WindowScene::Update()
{
    if (!App->input->GetMouseButton(SDL_BUTTON_RIGHT))
    {
        if (App->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
            guizmo_operation = ImGuizmo::TRANSLATE;
        if (App->input->GetKey(SDL_SCANCODE_E) == KeyState::KEY_REPEAT)
            guizmo_operation = ImGuizmo::ROTATE;
        if (App->input->GetKey(SDL_SCANCODE_R) == KeyState::KEY_REPEAT)
            guizmo_operation = ImGuizmo::SCALE;
    }
    ImGui::SetNextWindowDockID(App->editor->dock_main_id, ImGuiCond_FirstUseEver);
    if (ImGui::Begin((std::string(ICON_FA_GLOBE " ") + name).c_str(), &active))
    {
        focused = ImGui::IsWindowFocused();
        GuizmoOptionsController();
        ImGui::SameLine();
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
        ImGui::SameLine();
        ToolbarMenu();
        DrawScene();
        Controller();
        ImGui::End();
    }
}

void Hachiko::WindowScene::CleanUp() {}

void Hachiko::WindowScene::GuizmoOptionsController()
{
    if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_ARROWS_ALT, guizmo_operation == ImGuizmo::TRANSLATE))
    {
        guizmo_operation = ImGuizmo::TRANSLATE;
    }
    if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_UNDO, guizmo_operation == ImGuizmo::ROTATE))
    {
        guizmo_operation = ImGuizmo::ROTATE;
    }
    if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_EXPAND_ALT, guizmo_operation == ImGuizmo::SCALE))
    {
        guizmo_operation = ImGuizmo::SCALE;
    }

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();

    if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_HOME, guizmo_mode == ImGuizmo::LOCAL))
    {
        guizmo_mode = ImGuizmo::LOCAL;
    }
    if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_GLOBE, guizmo_mode == ImGuizmo::WORLD))
    {
        guizmo_mode = ImGuizmo::WORLD;
    }
}

void Hachiko::WindowScene::ToolbarMenu() const
{
    const float w = (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) * 0.5f - 30 - ImGui::GetCursorPosX();
    ImGui::Dummy(ImVec2(w, ImGui::GetTextLineHeight()));

    bool playing = !GameTimer::paused && GameTimer::running;

    if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_PAUSE, GameTimer::paused && GameTimer::running))
    {
        if (!GameTimer::paused)
        {
            GameTimer::Pause();
        }
    }

    if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_PLAY, !GameTimer::paused && GameTimer::running))
    {
        if (!GameTimer::running)
        {
            App->scene_manager->SaveScene("tmp_scene.scene");
            GameTimer::Start();
        }
        else if (GameTimer::paused)
        {
            GameTimer::Resume();
        }
    }

    if (ToolbarButton(App->editor->m_big_icon_font, ICON_FA_STOP, !GameTimer::running))
    {
        if (GameTimer::running)
        {
            GameTimer::Stop();
            App->scene_manager->LoadScene("tmp_scene.scene");
        }
    }

    ImGui::Separator();
}

void Hachiko::WindowScene::DrawScene()
{
    ImVec2 size = ImGui::GetContentRegionAvail();
    if (size.x != texture_size.x || size.y != texture_size.y)
    {
        texture_size = {size.x, size.y,};
        App->camera->OnResize(static_cast<unsigned>(texture_size.x), static_cast<unsigned>(texture_size.y));
    }

    ComponentCamera* camera = App->camera->GetMainCamera();

    // Top left corner of the image in imgui coords, top is y = 0
    guizmo_rect_origin = ImGui::GetCursorScreenPos();
    // Bottom left corner in opengl coordinates, bottom is y = 0
    texture_position = float2(guizmo_rect_origin.x, static_cast<float>(App->window->GetHeight()) - guizmo_rect_origin.y - texture_size.y);

    ImGui::Image((void*)static_cast<intptr_t>(App->renderer->GetTextureId()), size, ImVec2{0, 1}, ImVec2{1, 0});

    // TODO: Move to other section to take the input
    if (ImGui::BeginDragDropTarget())
    {
        if (auto* payload = ImGui::AcceptDragDropPayload("path"))
        {
            auto path = static_cast<const char*>(payload->Data);
            //App->scene_manager->LoadModel(path);
        }
        ImGui::EndDragDropTarget();
    }

    hovering = ImGui::IsWindowHovered();

    // TO AVOID Camera orbit
    if (ImGui::IsWindowFocused())
    {
        bool guizmo_enabled = !(App->input->GetMouseButton(SDL_BUTTON_RIGHT) || App->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_DOWN);
        ImGuizmo::Enable(guizmo_enabled);
    }

    constexpr bool transposed = true;
    float4x4 view = camera->GetViewMatrix(transposed);

    //TODO ADD look at when manipulating cube
    ImGuizmo::ViewManipulate(view.ptr(), 4, ImVec2(guizmo_rect_origin.x + texture_size.x - imguizmo_size.x, guizmo_rect_origin.y + texture_size.y - imguizmo_size.x), imguizmo_size, 0x10101010);

    if (ImGui::IsWindowFocused())
    {
        float4x4 new_camera_view = view.InverseTransposed();
        App->camera->GetMainCamera()->GetFrustum()->SetFrame(new_camera_view.Col(3).xyz(), -new_camera_view.Col(2).xyz(), new_camera_view.Col(1).xyz());
    }

    GameObject* selected_object = App->editor->GetSelectedGameObject();
    if (!selected_object)
        return;

    if (selected_object)
    {
        // Using GL format which means transposing them
        view = camera->GetViewMatrix(transposed);
        float4x4 projection = camera->GetProjectionMatrix(transposed);
        float4x4 model = selected_object->GetComponent<ComponentTransform>()->GetTransform().Transposed();
        float4x4 delta;

        ImGuizmo::SetRect(guizmo_rect_origin.x, guizmo_rect_origin.y, texture_size.x, texture_size.y);
        ImGuizmo::SetDrawlist();

        Manipulate(view.ptr(), projection.ptr(), guizmo_operation, guizmo_mode, model.ptr(), delta.ptr());

        using_guizmo = ImGuizmo::IsUsing();
        if (using_guizmo && !delta.IsIdentity())
        {
            // Transpose back again to store in our format
            model.Transpose();
            selected_object->GetComponent<ComponentTransform>()->SetGlobalTransform(model);
        }
    }
}

void Hachiko::WindowScene::Controller() const
{
    if (!using_guizmo && focused && App->input->GetMouseButton(SDL_BUTTON_LEFT))
    {
        Scene* scene = App->scene_manager->GetActiveScene();
        GameObject* picked = SelectObject(App->camera->GetMainCamera(), scene);
        if (picked)
            App->editor->SetSelectedGO(picked);
    }
}

Hachiko::GameObject* Hachiko::WindowScene::SelectObject(ComponentCamera* camera, Scene* scene) const
{
    // sdl -> (0, 0) top left, (w, h) bottom right
    // // imgui -> (0,0) botton left
    // plane -> (1,1) top right , (-1, -1) bottom left

    const ImVec2 mouse = ImGui::GetMousePos();
    const auto mouse_viewport_pos = float2(mouse.x - guizmo_rect_origin.x, mouse.y - guizmo_rect_origin.y);

    // Fit in range and coordinate direction
    const float x_normalized = mouse_viewport_pos.x / texture_size.x * 2.f - 1.f;
    const float y_normalized = -(mouse_viewport_pos.y / texture_size.y * 2.f - 1.f);

    const LineSegment line = camera->RayCast(x_normalized, y_normalized);
    GameObject* selected = scene->RayCast(line);

    return selected;
}

//TODO: Move to utils gui and add tooltips
bool Hachiko::WindowScene::ToolbarButton(ImFont* font, const char* font_icon, bool active) const
{
    const ImVec4 col_active = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
    const ImVec4 bg_color = active ? col_active : ImGui::GetStyle().Colors[ImGuiCol_Text];

    ImGui::SameLine();
    const auto frame_padding = ImGui::GetStyle().FramePadding;
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Text, bg_color);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, ImGui::GetStyle().FramePadding.y));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, frame_padding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

    ImGui::PushFont(font);
    active = ImGui::Button(font_icon);

    ImGui::PopFont();
    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(3);

    return active;
}
