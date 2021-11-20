#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleDebugDraw.h"

#include "SDL.h"
#include "glew.h"
#include "MathGeoLib.h"

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

// Destructor
ModuleRender::~ModuleRender()
{
}

bool ModuleRender::initGlew()
{
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		LOG("Glew library could not init %s\n", glewGetErrorString(err));
		return false;
	}
	else
		LOG("Using Glew %s", glewGetString(GLEW_VERSION));

	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	return true;
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

	context = SDL_GL_CreateContext(App->window->window);

	LOG("Creating Renderer context");
	if (context == nullptr)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
	SDL_GetWindowSize(App->window->window, (int*)&width, (int*)&height);
	glViewport(0, 0, width, height);
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	// 004 - Init OpenGL via SDL
	ret = initializeOpenGLviaSDL();

	if (ret != true) return false;
	
	// 004 - Init the glew library to wrap some sdl functions
	ret = initGlew();

	if (ret == true)
	{
		// Initialize some OpenGL global states
		glEnable(GL_DEPTH_TEST); // Enable depth test
		glEnable(GL_CULL_FACE); // Enable cull backward faces
		glFrontFace(GL_CCW); // Front faces will be counter clockwise

		glEnable(GL_DEBUG_OUTPUT); // Enable output callback
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		glDebugMessageCallback(&DebugMessageGL, nullptr); // Set the callback
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true); // Filter notifications
	}

	return true;
}

update_status ModuleRender::PreUpdate()
{
	// 004 - Clearing the screen
	int window_width, window_height;

	SDL_GetWindowSize(App->window->window, &window_width, &window_height);
	glViewport(0, 0, window_width, window_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	auto screen_surface = App->window->screen_surface;

	App->debug_draw->Draw(App->camera->GetViewSDL(), App->camera->GetProjectionSDL(), screen_surface->w, screen_surface->h);

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	// 004 - Destroy window and delete OPenGL context
	LOG("Destroying renderer");
	SDL_GL_DeleteContext(context);
	return true;
}
