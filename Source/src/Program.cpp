#include "Program.h"

#include "glew.h"

#include <stdlib.h>

Program::Program(unsigned vtx_shader, unsigned frg_shader)
{
	id = glCreateProgram();
	glAttachShader(id, vtx_shader);
	glAttachShader(id, frg_shader);
	glLinkProgram(id);
	int res;
	glGetProgramiv(id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*) malloc(len);
			glGetProgramInfoLog(id, len, &written, info);
			// LOG("Program Log Info: %s", info);
			free(info);
		}
		// Set id to 0 to indicate that it failed
		id = 0;
	}
}

void Program::Activate()
{
	glUseProgram(id);
}

void Program::Deactivate()
{
	glUseProgram(0);
}

void Program::CleanUp()
{
	glDeleteProgram(id);
}

void Program::BindUniformFloat4x4(const char* name, const float* data, bool transpose)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name), 1, transpose, data);
}

void Program::BindUniformFloat3(const char* name, const float* data)
{
	glUniform3fv(glGetUniformLocation(id, name), 1, data);
}

void Program::BindUniformFloat(const char* name, const float* data)
{
	glUniform1fv(glGetUniformLocation(id, name), 1, data);
}

void Program::BindUniformBool(const char* name, bool value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
}

void Program::BindUniformInts(const char* name, unsigned size, const int* data)
{
	glUniform1iv(glGetUniformLocation(id, name), size, data);
}
