#include "ModuleProgram.h"

#include "../Utils/Logger.h"
#include "glew.h"
#include "MathGeoLib.h"
#include "imgui.h"

ModuleProgram::ModuleProgram() {}

ModuleProgram::~ModuleProgram() {}

bool ModuleProgram::Init()
{
	CreateMainProgram();
	CreateSkyboxProgram();
	if (!main_program || !skybox_program)
		return false;
	return true;
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
		data = (char*) malloc(size + 1);
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
			char* info = (char*) malloc(len);
			glGetShaderInfoLog(shader_id, len, &written, info);
			LOG("Log Info: %s", info);
			free(info);
		}
		return 0;
	}
	return shader_id;
}

void ModuleProgram::CompileShaders(const char* vtx_shader_path, const char* frg_shader_path, unsigned& vtx_shader, unsigned& frg_shader)
{
	const char* vertex_source = LoadShaderSource(vtx_shader_path);
	const char* fragment_source = LoadShaderSource(frg_shader_path);

	vtx_shader = CompileShader(GL_VERTEX_SHADER, vertex_source);
	frg_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_source);

	delete vertex_source;
	delete fragment_source;
}

Program* ModuleProgram::CreateProgram(const char* vtx_shader_path, const char* frg_shader_path)
{
	unsigned fragment_shader_id;
	unsigned vertex_shader_id;
	CompileShaders(vtx_shader_path, frg_shader_path, vertex_shader_id, fragment_shader_id);

	if (vertex_shader_id == 0 || fragment_shader_id == 0)
		return nullptr;

	Program* program = new Program(vertex_shader_id, fragment_shader_id);

	if (program->GetId() == 0)
	{
		delete program;
		return nullptr;
	}

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return program;
}

Program* ModuleProgram::CreateMainProgram()
{
	main_program = CreateProgram("vertex.glsl", "fragment.glsl");

	light.position = float3(5.0f, 5.0f, 5.0f);
	light.direction = float3(5.0f, 5.0f, 5.0f);
	light.color = float3(1.0f, 1.0f, 1.0f);
	light.ambient_strength = 0.15f;
	light.directional = true;

	main_program->Activate();
	main_program->BindUniformBool("is_directional", light.directional);
	main_program->BindUniformFloat("ambient_strength", &light.ambient_strength);
	main_program->BindUniformFloat3("light_color", (float*) &light.color[0]);
	main_program->BindUniformFloat3("ligh_direction", (float*) &light.direction[0]);
	main_program->BindUniformFloat3("light_position", (float*) &light.position[0]);
	main_program->Deactivate();
	return main_program;
}

Program* ModuleProgram::CreateSkyboxProgram()
{
	skybox_program = CreateProgram("vertex_skybox.glsl", "fragment_skybox.glsl");
	return skybox_program;
}

bool ModuleProgram::CleanUp()
{
	main_program->CleanUp();
	delete main_program;
	skybox_program->CleanUp();
	delete skybox_program;
	return true;
}

void ModuleProgram::OptionsMenu()
{
	main_program->Activate();
	ImGui::SetNextItemWidth(50.0f);
	if (ImGui::SliderFloat("Ambient Value", &light.ambient_strength, 0.0f, 1.0f))
		main_program->BindUniformFloat("ambient_strength", &light.ambient_strength);

	if (ImGui::Checkbox("Directional", &light.directional))
		main_program->BindUniformBool("is_directional", light.directional);
	if (light.directional)
	{
		if (ImGui::SliderFloat3("Direction", &light.direction[0], -5.0f, 5.0f))
			main_program->BindUniformFloat3("ligh_direction", (float*) &light.direction[0]);
	}
	else
	{
		if (ImGui::SliderFloat3("Position", &light.position[0], -250.0f, 250.0f))
			main_program->BindUniformFloat3("light_position", (float*) &light.position[0]);
	}

	ImGuiColorEditFlags flag = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel;
	if (ImGui::ColorPicker3("Light Color", &light.color[0], flag))
		main_program->BindUniformFloat3("light_color", (float*) &light.color[0]);

	main_program->Deactivate();
}