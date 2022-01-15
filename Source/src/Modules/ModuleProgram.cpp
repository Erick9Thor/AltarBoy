#include "ModuleProgram.h"

#include "../Components/ComponentCamera.h"

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

	CreateCameraUBO();
	CreateLightsUBO();
	UpdateLights();
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

	main_program->Activate();
	main_program->BindUniformFloat3("light_color", (float*) &light.color[0]);
	main_program->Deactivate();
	return main_program;
}

Program* ModuleProgram::CreateSkyboxProgram()
{
	skybox_program = CreateProgram("vertex_skybox.glsl", "fragment_skybox.glsl");
	return skybox_program;
}



void ModuleProgram::CreateUBO(UBOPoints binding_point, unsigned size)
{
	glGenBuffers(1, &ubos[binding_point]);
	glBindBuffer(GL_UNIFORM_BUFFER, ubos[binding_point]);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, ubos[binding_point]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ModuleProgram::UpdateUBO(UBOPoints binding_point, unsigned size, void* data, unsigned offset)
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubos[binding_point]);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ModuleProgram::CreateCameraUBO()
{
	CreateUBO(UBOPoints::p_camera, sizeof(Camera));
}

void ModuleProgram::CreateLightsUBO()
{
	CreateUBO(UBOPoints::p_lights, sizeof(Lights));
}

bool ModuleProgram::CleanUp()
{
	main_program->CleanUp();
	delete main_program;
	skybox_program->CleanUp();
	delete skybox_program;
	return true;
}

void ModuleProgram::UpdateCamera(ComponentCamera* camera)
{
	Camera camera_data;
	camera_data.view = camera->GetViewMatrix();
	camera_data.proj = camera->GetProjectionMatrix();
	// TODO: Understand why camera_data.view.TranslatePart() does not give the position
	camera_data.pos = camera_data.view.RotatePart().Transposed().Transform(-camera_data.view.TranslatePart());

	UpdateUBO(UBOPoints::p_camera, sizeof(Camera), &camera_data);
}

void ModuleProgram::UpdateLights()
{
	Lights lights_data;
	// Ambient
	lights_data.ambient.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	// Directional
	lights_data.directional.direction = float4(light.direction, 0.0f);
	lights_data.directional.color = float4(light.color, 1.0f);
	// Spot (TODO: Make array)
	lights_data.point.position = float4(light.position, 0.0f);
	lights_data.point.color = float4(light.color, 1.0f);

	UpdateUBO(UBOPoints::p_lights, sizeof(Lights), &lights_data);
}

void ModuleProgram::OptionsMenu()
{
	main_program->Activate();
	ImGui::SliderFloat3("Direction", &light.direction[0], -5.0f, 5.0f);
	//ImGui::SliderFloat3("Position", &light.position[0], -250.0f, 250.0f);

	ImGuiColorEditFlags flag = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel;
	ImGui::ColorPicker3("Light Color", &light.color[0], flag);
}