#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleHardware.h"
#include "ModuleSceneManager.h"
#include "Model.h"
#include "SDL.h"

#include "ImGuiComponents/AppLog.h"

#include "glew.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"


ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
    IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableSetMousePos | ImGuiConfigFlags_DockingEnable;  // Enable Keyboard Controls
    io.WantSetMousePos = true;

    ImGui_ImplSDL2_InitForOpenGL(App->window->getWindow(), App->renderer->context);
    ImGui_ImplOpenGL3_Init();

    Logger->setShowConsole(!Logger->getShowConsole());
  
    // Setup style
    ImGui::StyleColorsDark();

    io.IniFilename = "imgui.ini";

	return true;
}

update_status ModuleEditor::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

    constexpr ImGuiWindowFlags dockspace_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Dockspace", nullptr, dockspace_flags);
    ImGui::PopStyleVar(3);

    const ImGuiID dockSpaceId = ImGui::GetID("DockspaceID");

    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    if (ImGui::BeginMainMenuBar())
    {
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

        if (ImGui::BeginMenu("Tools"))
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

    ImGui::EndMainMenuBar();

    ImGui::End();

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
    showMenu();
    Draw();
    return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
	return UPDATE_CONTINUE;
}

void ModuleEditor::Draw()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_MakeCurrent(App->window->getWindow(), App->renderer->context);
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return true;
}

void ModuleEditor::showMenu() 
{ 
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
            showFPSGraph();
        }
        ImGui::End();
    }

    if (show_model_window)
    {
        if (ImGui::Begin("Model", &show_model_window))
        {
            App->scene->DrawGui();
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

void ModuleEditor::showFPSGraph() {
    ImGui::Text("Limit Framerate:");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%i", App->GetFramerateLimit());

    char title[25];
    sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
    ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
    sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
    ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
}

void ModuleEditor::AddFPS(float fps, float ms)
{
    static int count = 0;

    if (count == FPS_LOG_SIZE)
    {
        for (int i = 0; i < FPS_LOG_SIZE - 1; ++i)
        {
            fps_log[i] = fps_log[i + 1];
            ms_log[i] = ms_log[i + 1];
        }
    }
    else
        ++count;

    fps_log[count - 1] = fps;
    ms_log[count - 1] = ms;
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

