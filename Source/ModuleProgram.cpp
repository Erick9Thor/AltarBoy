#include "ModuleProgram.h"

#include "glew.h"
#include "MathGeoLib.h"
#include "imgui.h"

ModuleProgram::ModuleProgram()
{
}

ModuleProgram::~ModuleProgram()
{
}

bool ModuleProgram::Init()
{
	static const bool transpose = GL_TRUE;
	const char* vertex_source = LoadShaderSource("vertex.glsl");
	const char* fragment_source = LoadShaderSource("fragment.glsl");

	vertex_shader_id = CompileShader(GL_VERTEX_SHADER, vertex_source);
	if (vertex_shader_id > 0)
		fragment_shader_id = CompileShader(GL_FRAGMENT_SHADER, fragment_source);
	if (fragment_shader_id > 0)
		program_id = CreateProgram(vertex_shader_id, fragment_shader_id);

	delete vertex_source;
	delete fragment_source;

	if (vertex_shader_id == 0 || fragment_shader_id == 0 || program_id == 0)
		return false;

	light.position = float3(5.0f, 5.0f, 5.0f);
	light.direction = float3(5.0f, 5.0f, 5.0f);
	light.color = float3(1.0f, 1.0f, 1.0f);
	light.ambient_strength = 0.15f;
	light.directional = true;

	Activate();
	BindUniformBool("is_directional", light.directional);
	BindUniformFloat("ambient_strength", &light.ambient_strength);
	BindUniformFloat3("light_color", (float*)&light.color[0]);
	BindUniformFloat3("ligh_direction", (float*)&light.direction[0]);
	BindUniformFloat3("light_position", (float*)&light.position[0]);
	Deactivate();

	return true;
}

bool ModuleProgram::CleanUp()
{
	glDeleteProgram(program_id);
	return true;
}

void ModuleProgram::Activate()
{
	glUseProgram(program_id);
}

void ModuleProgram::Deactivate()
{
	glUseProgram(0);
}

void ModuleProgram::BindUniformFloat4x4(const char* name, const float* data, bool transpose)
{
	glUniformMatrix4fv(glGetUniformLocation(program_id, name), 1, transpose, data);
}

void ModuleProgram::BindUniformFloat3(const char* name, const float* data)
{
	glUniform3fv(glGetUniformLocation(program_id, name), 1, data);
}

void ModuleProgram::BindUniformFloat(const char* name, const float* data)
{
	glUniform1fv(glGetUniformLocation(program_id, name), 1, data);
}

void ModuleProgram::BindUniformBool(const char* name, bool value)
{
	glUniform1i(glGetUniformLocation(program_id, name), value);
}

void ModuleProgram::OptionsMenu()
{
	Activate();
	ImGui::SetNextItemWidth(50.0f);
	if (ImGui::SliderFloat("Ambient Value", &light.ambient_strength, 0.0f, 1.0f))
		BindUniformFloat("ambient_strength", &light.ambient_strength);

	if (ImGui::Checkbox("Directional", &light.directional))
		BindUniformBool("is_directional", light.directional);
	if (light.directional) {
		if (ImGui::SliderFloat3("Direction", &light.direction[0], -5.0f, 5.0f))
			BindUniformFloat3("ligh_direction", (float*)&light.direction[0]);
	}
	else {
		if (ImGui::SliderFloat3("Position", &light.position[0], -250.0f, 250.0f))
			BindUniformFloat3("light_position", (float*)&light.position[0]);
	}

	ImGuiColorEditFlags flag = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel;
	if (ImGui::ColorPicker3("Light Color", &light.color[0], flag))
		BindUniformFloat3("light_color", (float*)&light.color[0]);

	Deactivate();
}

char* ModuleProgram::LoadShaderSource(const char* shader_file_name)
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

unsigned int ModuleProgram::CompileShader(unsigned type, const char* source)
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
		return 0;
	}
	return shader_id;
}

unsigned ModuleProgram::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
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
		return 0;
	}
	glDeleteShader(vtx_shader);
	glDeleteShader(frg_shader);
	return program_id;
}