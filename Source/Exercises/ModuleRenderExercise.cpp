#include "ModuleRenderExercise.h"

#include "../Program.h"
#include "../Application.h"
#include "../ModuleCamera.h"
#include "../ModuleTexture.h"
#include "../ModuleWindow.h"
#include "../GL/glew.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "SDL.h"

#include "il.h"
#include "ilu.h"


ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}
/*
unsigned int CreateSquareVBO() {
	float positions[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active 
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(0);
	// Texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return vbo;
}

unsigned int CreateSquareEBO()
{
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	return ebo;
}**/

bool ModuleRenderExercise::Init()
{

	/*LOG("Shaders: Creating program");
	shader_id = App->program->CreateProgram("Resources\\vertex-shader.txt", "Resources\\fragment-shader.txt");
	glUseProgram(shader_id);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	glViewport(0, 0, w, h);

	square_vbo = CreateSquareVBO();
	square_ebo = CreateSquareEBO();

	unsigned int img_id = App->texture->GetTextureData("Resources\\Lenna.png");

	// Generate
	glGenTextures(1, &App->texture->texture_id);
	glBindTexture(GL_TEXTURE_2D, App->texture->texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData());

	App->texture->CleanTexture(img_id);
	
	model = float4x4::identity;*/

	return true;
}

update_status ModuleRenderExercise::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::Update()
{
	// BIND BUFFER
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square_ebo);
	glUseProgram(shader_id);

	glUniformMatrix4fv(glGetUniformLocation(shader_id, "model"), 1, GL_FALSE, (const float*)&model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader_id, "view"), 1, GL_FALSE, (const float*)&App->camera->GetView()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader_id, "proj"), 1, GL_FALSE, (const float*)&App->camera->GetProjection()[0][0]);

	// BIND TEXTURES AND DRW
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, App->texture->texture_id);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	glDeleteBuffers(1, &square_ebo);
	glDeleteBuffers(1, &square_vbo);
	return true;
}
