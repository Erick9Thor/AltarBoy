#include "Globals.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "Model.h"
#include "SDL.h"

#include "ImGuiComponents/AppLog.h"

#include "glew.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"


ModuleGui::~ModuleGui()
{
}

// 006 - Creation Context
bool ModuleGui::Init()
{
	
	ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.IniFilename = "imgui.ini";
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableSetMousePos | ImGuiConfigFlags_DockingEnable;  // Enable Keyboard Controls
    io.WantSetMousePos = true;
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
    ImGui_ImplOpenGL3_Init("#version 330");
  
    // Setup style
    ImGui::StyleColorsDark();

	return true;
}

update_status ModuleGui::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

    return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleGui::Update()
{
    showMenu();
    Draw();
    return UPDATE_CONTINUE;
}

update_status ModuleGui::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// 006 - Destroying imgui contect
bool ModuleGui::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleGui::Draw()
{
    // Debug Draw on selected GameObject

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_MakeCurrent(App->window->window, App->renderer->context);
}

void ModuleGui::showMenu() {
    static bool showcase = false;
    static bool show_abaout = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New ...");
            ImGui::MenuItem("Load ...");

            ImGui::EndMenu();
        }

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

            if (ImGui::MenuItem("Abaout"))
                show_abaout = !show_abaout;

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
                showCameraWindow = !showCameraWindow;
            }
            if (ImGui::MenuItem("Texture info"))
            {
                showTextureWindow = !showTextureWindow;
            }
            if (ImGui::MenuItem("Model info"))
            {
                showModelWindow = !showModelWindow;
            }
            if (ImGui::MenuItem("FPS counter"))
            {
                fpsCounter = !fpsCounter;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window conf"))
        {
            showConfigWindow();
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (showCameraWindow)
    {
        if (ImGui::Begin("Camera", &showCameraWindow))
        {
            App->camera->DrawGui();

            ImGui::End();
        }
    }

    /* TODO: TEXTURE INFO
    if (showTextureWindow)
    {
        if (ImGui::Begin("Texture", &showCameraWindow))
        {
            App->texture->DrawGui();

            ImGui::End();
        }   
    }*/

    if (showModelWindow)
    {
        if (ImGui::Begin("Model", &showModelWindow))
        {
            App->renderer->house->DrawGui();

            ImGui::End();
        }
    }

    if (Logger->getShowConsole()) Logger->Draw();

    if (show_abaout) {
        showAbaoutInfo();
    }
}


void ModuleGui::showFPSGraph() {
    ImGui::Text("Limit Framerate:");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%i", App->GetFramerateLimit());

    char title[25];
    sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
    ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
    sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
    ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
}

void ModuleGui::showConfigWindow()
{
    bool fullscreen = App->window->IsFullscreen();
    bool resizable = App->window->IsResizable();

    if (ImGui::Checkbox("Fullscreen", &fullscreen))
        App->window->SetFullscreen(fullscreen);

    ImGui::SameLine();
    if (ImGui::Checkbox("Resizable", &resizable))
        App->window->SetResizable(resizable);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Restart to apply");
}

// TODO: add show hardware options
void ModuleGui::showHardwareInfo() {
    
}

void ModuleGui::showAbaoutInfo()
{
    bool show_another_window = true;
    ImGui::Begin("Aboout Window", &show_another_window);  
    ImGui::Text("Engine name: %s", TITLE);
    ImGui::Text("Version: %s", ENGINE_VERSION);
    ImGui::Separator();
    ImGui::Text("My Engine for C++ UPC MASTER!");
    ImGui::Text("Eric Torres Perramon");
    ImGui::End();
}
