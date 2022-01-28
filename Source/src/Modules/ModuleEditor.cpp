#include "../Globals.h"
#include "../Application.h"
#include "../Utils/Timer.h"

#include "ModuleEditor.h"
#include "ModuleFileSystem.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleHardware.h"
#include "ModuleInput.h"
#include "ModuleSceneManager.h"
#include "SDL.h"

#include "../GameObject.h"
#include "../Scene.h"
#include "../Components/ComponentCamera.h"

#include "../Importers/SceneImporter.h"

#include "ImGuizmo.h"

#include "glew.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <IconsFontAwesome5.h>
#include <imgui_internal.h>
#include "../Utils/Logger.h"

#include "ImGuiFileDialog.h"


ModuleEditor::ModuleEditor() {
	windows.push_back(&w_configuration);
	windows.push_back(&w_hierarchy);
	windows.push_back(&w_scene);
	windows.push_back(&w_inspector);
	windows.push_back(&w_about);
	windows.push_back(&w_console);
	windows.push_back(&w_resource);
	windows.push_back(&w_project);
	windows.push_back(&w_timers);
}

bool ModuleEditor::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	(void) io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		io.ConfigViewportsNoAutoMerge = false;
		io.ConfigViewportsNoTaskBarIcon = true;
	}

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		io.ConfigDockingTransparentPayload = true;
	}

	//TODO: ADD resource loader for fonts and icons
	io.Fonts->AddFontDefault();
	static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	m_big_icon_font = io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 10.0f, &icons_config, icons_ranges);
	m_small_icon_font = io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAR, 10.0f, &icons_config, icons_ranges);

	// Setup style
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->GetWindow(), App->renderer->GetGLContext());
	ImGui_ImplOpenGL3_Init();

	for (Window* panel : windows)
	{
		if (panel->active)
			panel->Init();
	}

	return true;
}

update_status ModuleEditor::PreUpdate(const float delta)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(const float delta)
{
	ImGui::CaptureMouseFromApp(true);
	ImGui::CaptureKeyboardFromApp(true);

	update_status retval = MainMenuBar();

	if (ImGuiFileDialog::Instance()->Display("LoadScene"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string file_path_name = ImGuiFileDialog::Instance()->GetFilePathName();

			//TODO: Make a function inside file sys to get relative path Assets/Scenes/X.scene
			std::string file_name_extension = App->file_sys->GetFileNameAndExtension(file_path_name.c_str());

			std::string file_path = std::string(ASSETS_FOLDER_SCENES) + "/" + file_name_extension;

			LOG("Loading scene: %s", file_path_name.c_str());
			App->scene_manager->LoadScene(file_path.c_str());
		}

		ImGuiFileDialog::Instance()->Close();
	}

	GenerateDockingSpace();

	for (Window* panel : windows)
	{
		if (panel->active)
			panel->Update();
	}

	RenderGui();
	return retval;
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
		ViewMenu();

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

			if (ImGui::MenuItem("Quit"))
				return UPDATE_STOP;

			ImGui::EndMenu();
		}

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
	Scene* current_scene = App->scene_manager->GetActiveScene();

	if (!ImGui::BeginMenu("File")) return;
	if (ImGui::MenuItem(ICON_FA_PLUS "New"))
	{
		App->scene_manager->CreateEmptyScene();
	}
	if (ImGui::MenuItem(ICON_FA_SAVE "Save", nullptr, false, true)) // TODO: Use internal timer to disable/enable
	{
		std::string temp_scene_file_path = std::string(ASSETS_FOLDER_SCENES) + "/" + "untitled" + SCENE_EXTENSION;
		App->scene_manager->SaveScene(temp_scene_file_path.c_str());
	}
	if (ImGui::MenuItem("Save as", nullptr, false, true)) // TODO: Use internal timer
	{
		ImGui::OpenPopup("Save scene");
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Load"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("LoadScene", "Load Scene", ".scene", ".");
	}

	ImGui::EndMenu();

	char file_name_buffer[32] = {'\0'};
	if (ImGui::BeginPopupModal("Save scene"))
	{
		ImGui::SetItemDefaultFocus();
		ImGui::InputText("File name", file_name_buffer, sizeof(file_name_buffer));
		if (ImGui::Button("Save"))
		{
			std::string temp_scene_file_path = std::string(ASSETS_FOLDER_SCENES) + "/" + file_name_buffer + SCENE_EXTENSION;
			App->scene_manager->SaveScene(temp_scene_file_path.c_str());
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void ModuleEditor::ViewMenu()
{
	if (ImGui::BeginMenu("View"))
	{
		ImGui::MenuItem(w_scene.name, "", &w_scene.active);
		ImGui::MenuItem(w_inspector.name, "", &w_inspector.active);
		ImGui::MenuItem(w_hierarchy.name, "", &w_hierarchy.active);
		ImGui::MenuItem(w_configuration.name, "", &w_configuration.active);
		ImGui::MenuItem(w_about.name, "", &w_about.active);
		ImGui::MenuItem(w_resource.name, "", &w_resource.active);
		ImGui::MenuItem(w_project.name, "", &w_project.active);
		ImGui::MenuItem(w_timers.name, "", &w_timers.active);
		ImGui::EndMenu();
	}
}

void ModuleEditor::EditMenu()
{
	// TODO: shortcuts
	const bool is_go_selected = GetSelectedGO() != nullptr;

	if (!ImGui::BeginMenu("Edit")) return;

	if (ImGui::MenuItem(ICON_FA_UNDO "Undo", nullptr, false, CanUndo()))
	{
		//TODO:
	}
	if (ImGui::MenuItem(ICON_FA_REDO "Redo", nullptr, false, CanRedo()))
	{
		//TODO:
	}
	ImGui::Separator();
	if (ImGui::MenuItem(ICON_FA_CLIPBOARD "Copy", nullptr, false, is_go_selected))
	{
		//TODO:
	}
	if (ImGui::MenuItem(ICON_FA_PASTE "Paste", nullptr, false, CanPaste()))
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

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	SDL_GL_MakeCurrent(App->window->GetWindow(), App->renderer->GetGLContext());
}

bool ModuleEditor::CleanUp()
{
	for (Window* panel : windows)
	{
		if (panel->active)
			panel->CleanUp();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}