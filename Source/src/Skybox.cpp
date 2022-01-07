#include "Skybox.h"

#include "Application.h"
#include "Program.h"

#include "Modules/ModuleProgram.h"
#include "Components/ComponentCamera.h"


Skybox::Skybox()
{
	const char* paths[6] = {
		"Skybox/right.jpg",
		"Skybox/left.jpg",
		"Skybox/top.jpg",
		"Skybox/bottom.jpg",			
		"Skybox/front.jpg",
		"Skybox/back.jpg"
	};
	texture = App->texture->LoadCubeMap(paths);

	float vertices[] = {
    // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
}

void Skybox::Draw(ComponentCamera* camera)
{
	// Use for optimized version (draw at the end) glDepthFunc(GL_LEQUAL);
	
    glDepthFunc(GL_ALWAYS);
	Program* program = App->program->GetSkyboxProgram();
	program->Activate();
	program->BindUniformFloat4x4("view", &camera->GetViewMatrix()[0][0]);
	program->BindUniformFloat4x4("proj", &camera->GetProjectionMatrix()[0][0]);
	// Draw skybox
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	program->Deactivate();	
	glDepthFunc(GL_LESS);
}