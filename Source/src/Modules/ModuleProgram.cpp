#include "ModuleProgram.h"

#include "../Application.h"
#include "../Components/ComponentCamera.h"
#include "../Components/ComponentDirLight.h"
#include "../Components/ComponentPointLight.h"
#include "../Components/ComponentSpotLight.h"
#include "../Resources/ResourceMaterial.h"

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
	CreateMaterialUBO();
	CreateLightsUBO();
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

void ModuleProgram::CreateMaterialUBO()
{
	CreateUBO(UBOPoints::p_material, sizeof(MaterialData));
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

void ModuleProgram::UpdateCamera(const ComponentCamera* camera)
{
	Camera camera_data;
	camera_data.view = camera->GetViewMatrix();
	camera_data.proj = camera->GetProjectionMatrix();
	// TODO: Understand why camera_data.view.TranslatePart() does not give the position
	camera_data.pos = camera_data.view.RotatePart().Transposed().Transform(-camera_data.view.TranslatePart());

	UpdateUBO(UBOPoints::p_camera, sizeof(Camera), &camera_data);
}

void ModuleProgram::UpdateMaterial(const ResourceMaterial* material)
{
	static int texture_slots[n_texture_slots] = {t_diffuse, t_specular};
	main_program->BindUniformInts("textures", n_texture_slots, &texture_slots[0]);
	
	MaterialData material_data;
	material_data.diffuse_color = material->diffuse_color;
	material_data.diffuse_flag = material->diffuse.loaded;
	material_data.specular_color = material->specular_color;
	material_data.specular_flag = material->specular.loaded;
	material_data.shininess = material->shininess;

	if (material->diffuse.loaded)
		App->texture->Bind(material->GetDiffuseId(), ModuleProgram::TextureSlots::t_diffuse);
	if (material->specular.loaded)
		App->texture->Bind(material->GetSpecularId(), ModuleProgram::TextureSlots::t_specular);

	UpdateUBO(UBOPoints::p_material, sizeof(MaterialData), &material_data);
}

void ModuleProgram::UpdateLights(const ComponentDirLight* dir_light, const std::vector<ComponentPointLight*>& point_lights, const std::vector<ComponentSpotLight*>& spot_lights)
{
	Lights lights_data;
	// Ambient
	lights_data.ambient = ambient_light;
	// DirectionalS
	if (dir_light->IsActive()) {
		lights_data.directional.direction = float4(dir_light->GetDirection(), 0.0f);
		lights_data.directional.color = dir_light->color;
		lights_data.directional.intensity = dir_light->intensity;
	}
	else
	{
		lights_data.directional.intensity = 0.0f;
	}

	// Point
	lights_data.n_points = 0;
	for (unsigned i = 0; i < point_lights.size(); ++i)
	{
		if (point_lights[i]->IsActive())
		{
			lights_data.points[lights_data.n_points].position = float4(point_lights[i]->GetPosition(), 0.0f);
			lights_data.points[lights_data.n_points].color = point_lights[i]->color;
			lights_data.points[lights_data.n_points].intensity = point_lights[i]->intensity;
			lights_data.points[lights_data.n_points].radius = point_lights[i]->radius;
			++lights_data.n_points;
			if (lights_data.n_points == MAX_POINT_LIGHTS)
				break;
		}
	}
	// Spot
	lights_data.n_spots = 0;
	for (unsigned i = 0; i < spot_lights.size(); ++i)
	{
		if (spot_lights[i]->IsActive())
		{
			lights_data.spots[lights_data.n_spots].position = float4(spot_lights[i]->GetPosition(), 0.0f);
			lights_data.spots[lights_data.n_spots].direction = float4(spot_lights[i]->GetDirection(), 0.0f);
			lights_data.spots[lights_data.n_spots].color = spot_lights[i]->color;
			lights_data.spots[lights_data.n_spots].inner = DegToRad(spot_lights[i]->inner);
			lights_data.spots[lights_data.n_spots].outer = DegToRad(spot_lights[i]->outer);
			lights_data.spots[lights_data.n_points].intensity = spot_lights[i]->intensity;
			lights_data.spots[lights_data.n_points].radius = spot_lights[i]->radius;
			++lights_data.n_spots;
			if (lights_data.n_spots == MAX_SPOT_LIGHTS)
				break;
		}
	}

	UpdateUBO(UBOPoints::p_lights, sizeof(Lights), &lights_data);
}

void ModuleProgram::OptionsMenu()
{
	ImGui::InputFloat("Ambient Intensity", &ambient_light.intensity);
	ImGuiColorEditFlags flag = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel;
	ImGui::ColorPicker3("Ambient Color", &ambient_light.color[0], flag);
}