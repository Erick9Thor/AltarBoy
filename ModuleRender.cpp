#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL/glew.h"

ModuleRender::ModuleRender()
{
}

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

// Destructor
ModuleRender::~ModuleRender()
{
}

// 05 - Simple function to load C/c++ filer on our case for upload fragmernt and vertx files
char* ModuleRender::LoadShaderSource(const char* shader_file_name)
{
	char* data = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, shader_file_name, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		data = (char*)malloc(size + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
	}
	return data;
}

// 05 - Create and compile shader examples
unsigned int ModuleRender::CompileShader(unsigned type, const char* source)
{
	unsigned shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, &source, 0);
	glCompileShader(shader_id);
	int res = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetShaderInfoLog(shader_id, len, &written, info);
			LOG("Log Info: %s", info);
			free(info);
		}
	}
	return shader_id;
}

// 05 - Finally we must create program object containing both shaders
unsigned ModuleRender::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{
	unsigned program_id = glCreateProgram();
	glAttachShader(program_id, vtx_shader);
	glAttachShader(program_id, frg_shader);
	glLinkProgram(program_id);
	int res;
	glGetProgramiv(program_id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetProgramInfoLog(program_id, len, &written, info);
			LOG("Program Log Info: %s", info);
			free(info);
		}
	}
	// after linking we can delete shaders
	glDeleteShader(vtx_shader);
	glDeleteShader(frg_shader);
	return program_id;
}

// 005 - This function must be called each frame for drawing the triangle
void ModuleRender::RenderVBO(unsigned vbo, unsigned program)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glUseProgram(program);
	// 1 triangle to draw = 3 vertices 
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

// 005 - This function must be called one time at creation of vertex buffer
unsigned  ModuleRender::CreateTriangleVBO()
{
	float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	return vbo;
}

// 005 - This function must be called one time at destruction of vertex buffer
void DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	// 004 - Init OpenGL via SDL
	ret = initializeOpenGLviaSDL();

	// 004 - Init the glew library to wrap some sdl functions
	ret = initGlew();

	if (ret == true)
	{
		//Initialize clear color
		glClearColor(0.4f, 0.4f, 0.4f, 1.f);

		// 004 - Initialize some OpenGL global states
		glEnable(GL_DEPTH_TEST); // Enable depth test
		glEnable(GL_CULL_FACE); // Enable cull backward faces
		glFrontFace(GL_CCW); // Front faces will be counter clockwise

		glEnable(GL_DEBUG_OUTPUT); // Enable output callback
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);


		glDebugMessageCallback(&DebugMessageGL, nullptr); // Set the callback
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true); // Filter notifications

		// 005 - Creating shaders and fragments

		char* vertex_shader = LoadShaderSource("vertex-shader.txt");
		char* fragment_shader = LoadShaderSource("fragment-shader.txt");

		unsigned int vertex_id = CompileShader(GL_VERTEX_SHADER, vertex_shader);
		unsigned int fragment_id = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

		program = CreateProgram(vertex_id, fragment_id);

		vbo = CreateTriangleVBO();
	}


	return true;
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

	context = SDL_GL_CreateContext(App->window->window);

	LOG("Creating Renderer context");
	if (context == nullptr)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
}

update_status ModuleRender::PreUpdate()
{
	// 004 - Clearing the screen
	int window_width, window_height;

	// Getting de X anad Y from actual window
	SDL_GetWindowSize(App->window->window, &window_width, &window_height);

	// Setting the current viewport
	glViewport(0, 0, window_width, window_height);

	// Clear values for the color buffers
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Clear buffers to preset values
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	//005 - Each frame called for rendering triangle
	RenderVBO(vbo, program);
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	// Update a window with the content of our current buffer
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	// 004 - Destroy window and delete OPenGL context
	LOG("Destroying renderer");
	SDL_GL_DeleteContext(context);
	// 005 
	DestroyVBO(vbo);
	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
	glViewport(0, 0, width, height);
}

