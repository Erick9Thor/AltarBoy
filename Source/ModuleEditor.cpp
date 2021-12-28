#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleHardware.h"
#include "ModuleInput.h"
#include "ModuleSceneManager.h"
#include "SDL.h"

#include "GameObject.h"
#include "Scene.h"
#include "ComponentCamera.h"

#include "ImGuiComponents/AppLog.h"

#include "glew.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <IconsFontAwesome5.h>
#include <imgui_internal.h>

ModuleEditor::ModuleEditor()
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
    IMGUI_CHECKVERSION();
	ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); 
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
   
    //TODO: ADD resource loader for fonts and icons
    io.Fonts->AddFontDefault();
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config; 
    icons_config.MergeMode = true; 
    icons_config.PixelSnapH = true;
    m_big_icon_font = io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 10.0f, &icons_config, icons_ranges);
    m_small_icon_font = io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAR, 10.0f, &icons_config, icons_ranges);

    io.WantSetMousePos = true;

    // Setup style
    ImGui::StyleColorsDark();

    io.IniFilename = "imgui.ini";

    Logger->setShowConsole(!Logger->getShowConsole());

    ImGui_ImplSDL2_InitForOpenGL(App->window->GetWindow(), App->renderer->GetGLContext());
    ImGui_ImplOpenGL3_Init();

	return true;
}

update_status ModuleEditor::PreUpdate(const float delta)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(const float delta)
{
    GenerateDockingSpace();

    update_status retval = MainMenuBar();
    SceneBar();

    showWindowsViewports();

    SceneViewport();

    RenderGui();
    return UPDATE_CONTINUE;
}

void ModuleEditor::SceneBar()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    float height = ImGui::GetFrameHeight();
    if (ImGui::BeginViewportSideBar("##SceneEditor", NULL, ImGuiDir_Up, height, window_flags)) {
        if (ImGui::BeginMenuBar()) {
            float w = (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) * 0.5f - 30 - ImGui::GetCursorPosX();
            ImGui::Dummy(ImVec2(w, ImGui::GetTextLineHeight()));

            ToolbarButton(m_big_icon_font, ICON_FA_PLAY);
            ToolbarButton(m_big_icon_font, ICON_FA_PAUSE);
            ToolbarButton(m_big_icon_font, ICON_FA_STEP_FORWARD);
            ImGui::EndMenuBar();
        }
    }
    ImGui::End();
}

update_status ModuleEditor::MainMenuBar()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 4));
    if (ImGui::BeginMainMenuBar())
    {
        FileMenu();
        EditMenu();
        GoMenu();

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Documentation"))
                App->RequestBrowser("https://github.com/Erick9Thor/Engine3D/wiki");

            if (ImGui::MenuItem("Download latest"))
                App->RequestBrowser("https://github.com/Erick9Thor/Engine3D/releases");

            if (ImGui::MenuItem("Report a bug"))
                App->RequestBrowser("https://github.com/Erick9Thor/Engine3D/issues");

            if (ImGui::MenuItem("Download source!"))
                App->RequestBrowser("https://www.youtube.com/watch?v=dQw4w9WgXcQ");

            if (ImGui::MenuItem("About"))
                show_abaout = !show_abaout;

            if (ImGui::MenuItem("Quit"))
                return UPDATE_STOP;

            ImGui::EndMenu();
        }

        ViewMenu();

        ImGui::PopStyleVar(2);

        

        ImGui::EndMainMenuBar();
    }


    return UPDATE_CONTINUE;
}

void ModuleEditor::GenerateDockingSpace()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiWindowFlags host_window_flags = 0;
    host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
    host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        host_window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Window", nullptr, host_window_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, nullptr);
    ImGui::End();
}

void ModuleEditor::FileMenu()
{
    // TODO: shortcuts

    if (!ImGui::BeginMenu("File")) return;
    if (ImGui::MenuItem(ICON_FA_PLUS "New"))
    {
        //TODO: Create a new scene
    }
    if (ImGui::MenuItem(ICON_FA_SAVE "Save", nullptr, false, !App->scene_manager->isSceneRuning()))
    {
        //TODO: Save the current scene
    }
    if (ImGui::MenuItem("Save as", nullptr, false, !App->scene_manager->isSceneRuning()))
    {
        //TODO: Save as
    }
    ImGui::EndMenu();
}

void ModuleEditor::ViewMenu()
{
    if (ImGui::BeginMenu("View"))
    {
        if (ImGui::MenuItem("Console"))
        {
            Logger->setShowConsole(!Logger->getShowConsole());
        }
        if (ImGui::MenuItem("Camera"))
        {
            show_camera_window = !show_camera_window;
        }
        if (ImGui::MenuItem("FPS counter"))
        {
            show_fps_counter = !show_fps_counter;
        }
        ImGui::EndMenu();
    }
}

void ModuleEditor::ToolbarButton(ImFont* font, const char* font_icon)
{
    const ImVec4 col_active = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
    const ImVec4 bg_color = false ? col_active : ImGui::GetStyle().Colors[ImGuiCol_Text];

    ImGui::SameLine();
    auto frame_padding = ImGui::GetStyle().FramePadding;
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Text, bg_color);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, ImGui::GetStyle().FramePadding.y));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, frame_padding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

    ImGui::PushFont(font);
    if (ImGui::Button(font_icon)) {
        // TODO: function
    }

    ImGui::PopFont();
    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(3);
}

void ModuleEditor::SceneViewport()
{
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Scene");

    const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    float x = viewportPanelSize.x, y = viewportPanelSize.y;

    App->renderer->WindowResized(x, y);
    App->camera->SetAspectRatio((unsigned)x, (unsigned)y);

    ImGui::Image((ImTextureID)(App->camera->getMainCamera()->GetTextureId()), ImVec2{ x, y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End();
    ImGui::PopStyleVar();
}

void ModuleEditor::EditMenu()
{
    // TODO: shortcuts
    const bool is_go_selected = getSelectedGO() != nullptr;

    if (!ImGui::BeginMenu("Edit")) return;

    if (ImGui::MenuItem(ICON_FA_UNDO "Undo", nullptr, false, canUndo()))
    {
        //TODO: 
    }
    if (ImGui::MenuItem(ICON_FA_REDO "Redo", nullptr, false, canRedo()))
    {
        //TODO: 
    }
    ImGui::Separator();
    if (ImGui::MenuItem(ICON_FA_CLIPBOARD "Copy", nullptr, false, is_go_selected))
    {
        //TODO: 
    }
    if (ImGui::MenuItem(ICON_FA_PASTE "Paste", nullptr, false, canPaste()))
    {
        //TODO: 
    }
    if (ImGui::MenuItem(ICON_FA_CLONE "Duplicate", nullptr, false, is_go_selected))
    {
        //TODO: 
    }
    ImGui::EndMenu();
}

void ModuleEditor::GoMenu()
{
    if (!ImGui::BeginMenu("GameObject")) return;

    ImGui::EndMenu();
}

void ModuleEditor::RenderGui()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_MakeCurrent(App->window->GetWindow(), App->renderer->GetGLContext());
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return true;
}

void ModuleEditor::showWindowsViewports() 
{ 
    if (show_hirarchy) {
        if (ImGui::Begin("Hierarchy", &show_hirarchy)) {
            DrawHierarchyTree(App->scene_manager->GetRoot());
        }
        ImGui::End();
    }

    if (show_inspector_window) {
        if (ImGui::Begin("Inspector", &show_inspector_window))
        {
            InspectorDrawGameObject(selected_go);
        }
        ImGui::End();
    }

    if (show_camera_window)
    {
        if (ImGui::Begin("Camera", &show_camera_window))
        {
            App->camera->DrawGui();
        }
        ImGui::End();
    }

    if (show_fps_counter)
    {
        if (ImGui::Begin("FPS counter", &show_fps_counter))
        {
            App->renderer->FpsGraph();
        }
        ImGui::End();
    }

    if (Logger->getShowConsole()) Logger->Draw();

    if (show_abaout) {
        if (ImGui::Begin("Aboout Window", &show_abaout))
        {
            showAbaoutInfo();
        }
        ImGui::End();
    }
}

void ModuleEditor::DrawHierarchyTree(GameObject* root)
{
    DrawGOChilds(root);
}

void ModuleEditor::DrawGOChilds(GameObject* root)
{
    for (GameObject* game_object : root->childs) {
        DrawGameObject(game_object);
    }
}

void ModuleEditor::DrawGameObject(GameObject* game_object)
{
    ShowGameObject(game_object);

    if (ImGui::IsItemHovered())
    {
        if (App->input->GetMouseButton(SDL_BUTTON_LEFT))
            selected_go = game_object;

        if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) )
        {
            ImGui::OpenPopup(game_object->name.c_str());
        }  
    }

    // TODO: Make robust to repeted game object names
    if (ImGui::BeginPopup(game_object->name.c_str())) {
        // Alternativs: ImGui::Selectable, ImGuiHelper::ValueSelection
        // TODO: Open options to create/destroy new object or move up down in the list of childs
        if (ImGui::MenuItem("Add Gameojbect"))
        {
            App->scene_manager->GetActiveScene()->CreateNewGameObject("Test", game_object);
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("Delete Gameojbect"))
        {
            // TODO: Make it work
            //game_object->Destroy();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("Move Up"))
        {
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("Move Down"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    

    if (game_object->hierarchy_open == true)
    {
        DrawGOChilds(game_object);
        ImGui::TreePop();
    }
}

void ModuleEditor::ShowGameObject(GameObject* game_object)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if (game_object->childs.empty())
    {
        flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    bool nodeOpen = ImGui::TreeNodeEx(game_object, flags, game_object->name.c_str());
    game_object->hierarchy_open = game_object->childs.empty() ? false : nodeOpen;
}

void ModuleEditor::InspectorDrawGameObject(GameObject* game_object)
{
    if (game_object != nullptr) {

        char go_name[50];
        strcpy_s(go_name, 50, game_object->name.c_str());
        ImGuiInputTextFlags name_input_flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
        if (ImGui::InputText("###", go_name, 50, name_input_flags))
            game_object->name = go_name;

        std::vector<Component*> go_components = game_object->GetComponents();
        for (vector<Component*>::iterator it = go_components.begin(); it != go_components.end(); ++it)
            (*it)->DrawGui();

        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("Add Component Popup");
        }

        if (ImGui::BeginPopup("Add Component Popup"))
        {
            if (ImGui::MenuItem("Mesh"))
            {
                game_object->CreateComponent(Component::Type::Mesh);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Material"))
            {
                game_object->CreateComponent(Component::Type::Material);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Camera"))
            {
                game_object->CreateComponent(Component::Type::Camera);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}

void ModuleEditor::showAbaoutInfo()
{
    ImGui::Text("Engine name: %s", TITLE);
    ImGui::Text("Version: %s", ENGINE_VERSION);
    ImGui::Separator();
    ImGui::Text("My Engine for C++ UPC MASTER!");
    ImGui::Text("Eric Torres Perramon");
    ImGui::Separator();
    ImGui::Text("SDL Version: %d.%d.%d", App->hw->GetInfo().sdl_version.major,
        App->hw->GetInfo().sdl_version.minor, App->hw->GetInfo().sdl_version.patch);
    ImGui::Separator();
    ImGui::Text("CPUs: %d", App->hw->GetInfo().n_cpu);
    ImGui::Text("System RAM: %.1f Gb", App->hw->GetInfo().ram_gb);
    ImGui::Separator();
    ImGui::Text("GPU: %s", App->hw->GetInfo().gpu);
    ImGui::Text("Brand: %s", App->hw->GetInfo().gpu_brand);
    ImGui::Text("VRAM Budget: %.1f Mb", (float) App->hw->GetInfo().vram_mb_budget);
    ImGui::Text("Vram Avaliable:  %.1f Mb", (float) App->hw->GetInfo().vram_mb_free);
    ImGui::Text("Vram Usage:  %.1f Mb", (float) (App->hw->GetInfo().vram_mb_budget - App->hw->GetInfo().vram_mb_free));
    ImGui::Separator();
    ImGui::Text("DevIL lib version %d", IL_VERSION);
}

