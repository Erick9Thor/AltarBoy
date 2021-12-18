#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleDebugDraw.h"

#include "ModuleCamera.h"
#include "ComponentCamera.h"

#include "SDL.h"
#include "glew.h"

#include "MathGeoLib.h"
#include "il.h"
#include "ilu.h"

#include "Leaks.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

void __stdcall DebugMessageGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (id == 131185 || id == 131204) return;

	char tmp_string[4096];

	const char* tmp_source = "", * tmp_type = "";

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             tmp_source = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   tmp_source = "Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: tmp_source = "Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     tmp_source = "Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     tmp_source = "Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           tmp_source = "Other"; break;
	};

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               tmp_type = "Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: tmp_type = "Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  tmp_type = "Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         tmp_type = "Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         tmp_type = "Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              tmp_type = "Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          tmp_type = "Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           tmp_type = "Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               tmp_type = "Other"; break;
	};


	const char* tmp_severity = "";
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         tmp_severity = "high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       tmp_severity = "medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          tmp_severity = "low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: tmp_severity = "notification"; break;
	};

	sprintf_s(tmp_string, 4095, "<Source:%s> <Type:%s> <Severity:%s> <ID:%d> <Message:%s>\n", tmp_source, tmp_type, tmp_severity, id, message);
	OutputDebugString(tmp_string);
}

ModuleRender::ModuleRender()
{
}

ModuleRender::~ModuleRender()
{
	SDL_GL_DeleteContext(context);
}

bool ModuleRender::initializeOpenGLviaSDL()
{

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG); // enable context debug

	LOG("[M_Render] Creating Renderer context");
	context = SDL_GL_CreateContext(App->window->getWindow());

	if (context == nullptr)
	{
		LOG("[M_Render] OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

bool ModuleRender::initGlew()
{
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		LOG("[M_Render] Glew library could not init %s\n", glewGetErrorString(err));
		return false;
	}
	else
		LOG("[M_Render] Using Glew %s", glewGetString(GLEW_VERSION));

	LOG("[M_Render] Vendor: %s", glGetString(GL_VENDOR));
	LOG("[M_Render] Renderer: %s", glGetString(GL_RENDERER));
	LOG("[M_Render] OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("[M_Render] GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	return true;
}

bool ModuleRender::Init()
{
	LOG("[M_Render] Creating 3D Renderer context");
	bool ret = true;

	ret = initializeOpenGLviaSDL();

	if (ret != true) return false;

	ret = initGlew();

	if (ret == true) {

		glEnable(GL_DEPTH_TEST); // Enable depth test
		glEnable(GL_CULL_FACE); // Enable cull backward faces
		glFrontFace(GL_CCW); // Front faces will be counter clockwise

		glEnable(GL_DEBUG_OUTPUT); // Enable output callback
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(&DebugMessageGL, nullptr); // Set the callback
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true); // Filter notifications

		int w, h;
		SDL_GetWindowSize(App->window->getWindow(), &w, &h);
		glViewport(0, 0, w, h);
	}

	GenerateFrameBuffer();

	return true;
}

update_status ModuleRender::PreUpdate()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	ImGui::Begin("Scene");

	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const ImVec2 newViewportPanelSize = ImGui::GetContentRegionAvail();
	if (viewportPanelSize.x != newViewportPanelSize.x || viewportPanelSize.y != newViewportPanelSize.y) {
		viewportPanelSize.x = newViewportPanelSize.x;
		viewportPanelSize.y = newViewportPanelSize.y;

		//Resize textures
		glBindTexture(GL_TEXTURE_2D, fb_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportPanelSize.x, viewportPanelSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportPanelSize.x, viewportPanelSize.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		App->camera->SetAspectRatio(viewportPanelSize.x, viewportPanelSize.y);
	}

	glViewport(0, 0, viewportPanelSize.x, viewportPanelSize.y);

	float4x4 view = App->camera->getViewPortCamera()->GetViewMatrix(false);
	float4x4 proj = App->camera->getViewPortCamera()->GetProjectionMatrix(false);

	App->debug_draw->Draw(view, proj, App->window->getScreenSurface()->w, App->window->getScreenSurface()->h);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ImGui::Image((ImTextureID)fb_texture, ImVec2{ newViewportPanelSize.x, newViewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{	
	SDL_GL_SwapWindow(App->window->getWindow());
	
	return UPDATE_CONTINUE;
}

void ModuleRender::GenerateFrameBuffer()
{
	//Generating buffers for target rendering
	glGenFramebuffers(1, &frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

	//Generating texture to render to
	glGenTextures(1, &fb_texture);
	glBindTexture(GL_TEXTURE_2D, fb_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Generating the depth & stencil buffer
	glGenRenderbuffers(1, &depth_stencil_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_buffer);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Configuring frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_texture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("[M_Render] Error creating frame buffer");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("[M_Render] Destroying renderer");
	SDL_GL_DeleteContext(context);
	return true;
}

void ModuleRender::WindowResized(unsigned int width, unsigned int height)
{
	SDL_GetWindowSize(App->window->getWindow(), (int*)&width, (int*)&height);
	glViewport(0, 0, width, height);
}

