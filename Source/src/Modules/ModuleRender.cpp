#include "../Globals.h"
#include "../Application.h"
#include "../Utils/Logger.h"

#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleDebugDraw.h"
#include "ModuleSceneManager.h"
#include "ModuleEditor.h"
#include "../Scene.h"
#include "../Skybox.h"
#include "../Quadtree.h"
#include "../GameObject.h"

#include "../Components/ComponentCamera.h"

#include "SDL.h"
#include "glew.h"

#include "MathGeoLib.h"
#include "il.h"
#include "ilu.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

ModuleRender::ModuleRender()
{
}

ModuleRender::~ModuleRender()
{
}

void __stdcall OurOpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

bool ModuleRender::Init()
{
	LOG("Init Module render");

	CreateContext();

	RetrieveGpuInfo();
	RetrieveLibVersions();

	SetGLOptions();
	GenerateFrameBuffer();

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT); // Enable output callback
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&OurOpenGLErrorFunction, nullptr); // Set the callback
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true); // Filter notifications
#endif

	fps_log = std::vector<float>(n_bins);
	ms_log = std::vector<float>(n_bins);

	return true;
}

void ModuleRender::GenerateFrameBuffer()
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
		LOG("Error creating frame buffer");
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ModuleRender::ResizeFrameBuffer(int heigth, int width)
{
	glBindTexture(GL_TEXTURE_2D, fb_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, heigth, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, heigth, width);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void ModuleRender::ManageResolution(ComponentCamera* camera)
{
	unsigned res_x, res_y;
	camera->GetResolution(res_x, res_y);
	if (res_x != fb_height || res_y != fb_width) {		
		ResizeFrameBuffer(res_x, res_y);
		glViewport(0, 0, res_x, res_y);
		fb_height = res_x;
		fb_width = res_y;		
	}	
}

void ModuleRender::CreateContext()
{
	LOG("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG); // enable context debug

	context = SDL_GL_CreateContext(App->window->GetWindow());
	GLenum err = glewInit();

	clear_color = float4(0.1f, 0.1f, 0.1f, 1.0f);
}

void ModuleRender::SetGLOptions()
{
	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise
	glEnable(GL_STENCIL_TEST); // Enable stencil test
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Only replace stencil value if stencil and depth tests pass
}

update_status ModuleRender::Update(const float delta)
{
	ComponentCamera* camera = App->camera->GetMainCamera();
	
	// Using debug camera to test culling

	App->program->UpdateCamera(camera);
	Scene* active_scene = App->scene_manager->GetActiveScene();
	ComponentCamera* culling = active_scene->GetCullingCamera();

	ComponentDirLight* dir_light = nullptr;
	if (active_scene->dir_lights.size() > 0)
		dir_light = active_scene->dir_lights[0];

	App->program->UpdateLights(dir_light, active_scene->point_lights, active_scene->spot_lights);

	Draw(App->scene_manager->GetActiveScene(), camera, culling);
	return UPDATE_CONTINUE;
}

void ModuleRender::Draw(Scene* scene, ComponentCamera* camera, ComponentCamera* culling)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

	ManageResolution(camera);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 1, 0XFF);
	glStencilMask(0x00); // Prevent background from filling stencil

	if (draw_skybox)
		scene->GetSkybox()->Draw(camera);
	else
		glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
	
	float4x4 view = camera->GetViewMatrix();
	float4x4 proj = camera->GetProjectionMatrix();

	glStencilMask(0XFF);

	App->debug_draw->Draw(view, proj, fb_height, fb_width);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	//Draw alternatives 0 optiomization, no quadtree
	// GameObject* root = scene->GetRoot();
	//root->DrawAll(camera);
	//render_list.Update(culling, root);
	
	render_list.Update(culling, scene->GetQuadtree()->GetRoot());
	Program* program = App->program->GetMainProgram();
	program->Activate();

	GameObject* selected_go = App->editor->GetSelectedGO();
	RenderTarget* outline_target = nullptr;
	for (RenderTarget& target : render_list.GetNodes())
	{
		target.game_object->Draw(camera, program);
		if (selected_go && target.game_object == selected_go)
		{
			outline_target = &target;
		}
	}		
	program->Deactivate();

	if (outline_selection && outline_target)
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0XFF);
		glStencilMask(0X00);
		glDisable(GL_DEPTH_TEST);		

		Program* outline_program = App->program->GetStencilProgram();
		outline_program->Activate();
		outline_target->game_object->DrawStencil(camera, outline_program);
		outline_program->Deactivate();

		glStencilMask(0XFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


update_status ModuleRender::PostUpdate(const float delta)
{
	SDL_GL_SwapWindow(App->window->GetWindow());
	AddFrame(delta);

	return UPDATE_CONTINUE;
}

void GLOptionCheck(GLenum option, bool enable)
{
	if (enable)
		glEnable(option);
	else
		glDisable(option);
}

void ModuleRender::OptionsMenu()
{
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Draw Options");
	ImGui::Checkbox("Debug Draw", &App->debug_draw->debug_draw);
	ImGui::Checkbox("Quadtree", &App->debug_draw->draw_quadtree);

	ImGui::Checkbox("Skybox", &draw_skybox);
	if (!draw_skybox)
		ImGuiUtils::CompactColorPicker("Background Color", &clear_color[0]);
}

void ModuleRender::PerformanceMenu()
{
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &vram_free);
	float vram_free_mb = vram_free / 1024.0f;
	float vram_usage_mb = gpu.vram_budget_mb - vram_free_mb;
	ImGui::Text("VRAM Budget: %.1f Mb", gpu.vram_budget_mb);
	ImGui::Text("Vram Usage:  %.1f Mb", vram_usage_mb);
	ImGui::Text("Vram Avaliable:  %.1f Mb", vram_free_mb);
	ImGui::Separator();
	FpsGraph();
}

void ModuleRender::FpsGraph()
{
	ImGui::Text("Fps: %.1f", current_fps);

	char title[25];
	sprintf_s(title, 25, "Framerate %.1f", current_fps);
	ImGui::PlotHistogram("##framerate", &fps_log[0], (int) fps_log.size(), 0, title, 0.0f, 1000.f, ImVec2(310, 100));
	sprintf_s(title, 25, "Milliseconds %0.1f", current_ms);
	ImGui::PlotHistogram("##milliseconds", &ms_log[0], (int) ms_log.size(), 0, title, 0.0f, 20.0f, ImVec2(310, 100));
}

void ModuleRender::AddFrame(const float delta)
{
	static const float update_frequency_seconds = 0.5f;
	static int filled_bins = 0;
	static int frames = 0;
	static float time = 0;

	++frames;
	time += delta;

	if (time >= update_frequency_seconds)
	{
		if (filled_bins == n_bins)
		{
			for (int i = 0; i < n_bins - 1; ++i)
			{
				fps_log[i] = fps_log[i + 1];
				ms_log[i] = ms_log[i + 1];
			}
		}
		else
		{
			++filled_bins;
		}
		fps_log[filled_bins - 1] = float(frames) / time;
		current_fps = fps_log[filled_bins - 1];
		ms_log[filled_bins - 1] = time * 1000.0f / float(frames);
		current_ms = ms_log[filled_bins - 1];
		time = 0;
		frames = 0;
	}
}

void ModuleRender::RetrieveLibVersions()
{
	gl.glew = (unsigned char*) glewGetString(GLEW_VERSION);
	gl.opengl = (unsigned char*) glGetString(GL_VERSION);
	gl.glsl = (unsigned char*) glGetString(GL_SHADING_LANGUAGE_VERSION);

	LOG("Using Glew %s", gl.glew);
	LOG("OpenGL version supported %s", gl.opengl);
	LOG("GLSL: %s", gl.glsl);
}

void ModuleRender::RetrieveGpuInfo()
{
	gpu.name = (unsigned char*) glGetString(GL_RENDERER);
	gpu.brand = (unsigned char*) glGetString(GL_VENDOR);

	int vram_budget;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &vram_budget);
	gpu.vram_budget_mb = (float) vram_budget / 1024.0f;
}

bool ModuleRender::CleanUp()
{
	//LOG("Destroying renderer");
	SDL_GL_DeleteContext(context);

	return true;
}

void __stdcall OurOpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	const char *tmp_source = "", *tmp_type = "", *tmp_severity = "";
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		tmp_source = "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		tmp_source = "Window System";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		tmp_source = "Shader Compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		tmp_source = "Third Party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		tmp_source = "Application";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		tmp_source = "Other";
		break;
	};
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		tmp_type = "Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		tmp_type = "Deprecated Behaviour";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		tmp_type = "Undefined Behaviour";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		tmp_type = "Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		tmp_type = "Performance";
		break;
	case GL_DEBUG_TYPE_MARKER:
		tmp_type = "Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		tmp_type = "Push Group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		tmp_type = "Pop Group";
		break;
	case GL_DEBUG_TYPE_OTHER:
		tmp_type = "Other";
		break;
	};
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		tmp_severity = "high";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		tmp_severity = "medium";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		return;
		// case GL_DEBUG_SEVERITY_NOTIFICATION: tmp_severity = "notification"; break;
	default:
		return;
	};
	LOG("<Source:%s> <Type:%s> <Severity:%s> <ID:%d> <Message:%s>\n", tmp_source, tmp_type, tmp_severity, id, message);
}