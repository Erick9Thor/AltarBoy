#include "Globals.h"
#include "Model.h"
#include "Application.h"

#include "glew.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "il.h"
#include "ilu.h"
#include "imgui.h"

Model::Model()
{
}

Model::~Model()
{
	CleanUp();
}

void Model::Draw()
{
	if (loaded) {
		for (Mesh& mesh : meshes) {
			mesh.Draw(matrix, textures);
		}
	}	
}

void Model::Load(const std::string& model_path)
{
	if (loaded)
		CleanUp();

	matrix = float4x4::identity; // Reset on load
	position = matrix.TranslatePart();
	rotation = matrix.ToEulerXYZ();
	scale = matrix.ExtractScale();

	info.path = model_path.substr(0, model_path.find_last_of("/\\") + 1);
	info.file_name = model_path.substr(model_path.find_last_of("/\\") + 1);
	info.name = info.file_name.substr(0, std::string::size_type(info.file_name.find_last_of('.')));
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(model_path, aiProcess_Triangulate);
	if (scene)
	{		
		bool success = true;
		LOG("Loading %s model", info.file_name.c_str())
		success = LoadTextures(scene);
		if(success)
			success = LoadMeshes(scene);
		if (success) {
			GenerateAABB();
			loaded = true;
		}
			
		else {
			LOG("Could not load model %s", info.file_name.c_str())
			CleanUp();
		}			
	}
	else
	{
		LOG("Error loading file %s: %s", info.file_name.c_str(), aiGetErrorString());
	}
	importer.FreeScene();
}

bool Model::LoadTextures(const aiScene* scene)
{	
	textures.reserve(scene->mNumMaterials);
	LOG("Loading %d textures", scene->mNumMaterials)
	for (unsigned i = 0; i < scene->mNumMaterials; i++)
	{
		// Atm we only support loading a single texture so index is hardcoded to 0
		if (!LoadTexture(scene->mMaterials[i]))
			return false;		
	}
	return true;
}

bool Model::LoadTexture(const aiMaterial* material)
{
	static const int index = 0;
	aiString file;
	if (material->GetTexture(aiTextureType_DIFFUSE, index, &file) == AI_SUCCESS)
	{
		std::string model_texture_path(file.data);
		std::string texture_file = model_texture_path.substr(model_texture_path.find_last_of("/\\") + 1);
		std::string default_path("Textures\\");

		Texture texture;
		texture = App->texture->Load(file.data);
		if (!texture.loaded) {
			LOG("Failed to load texture from model params: %s", texture.path.c_str())
				texture = App->texture->Load((info.path + texture_file).c_str());
		}
		if (!texture.loaded)
		{
			LOG("Failed to load texture from model file path: %s", texture.path.c_str())
				texture = App->texture->Load((default_path + texture_file).c_str());
		}
		if (!texture.loaded) {
			LOG("Failed to load texture from default: %s", texture.path.c_str())
				LOG("Could not find texture %s")
				return false;
		}
		textures.push_back(texture);
		LOG("Loaded texture from %s", texture.path.c_str())
	}
	else {
		LOG("Error loading textures %s: %s", info.file_name.c_str(), aiGetErrorString());
		return false;
	}
	return true;
}

void Model::GenerateAABB()
{
	bounding_box.SetNegativeInfinity(); // Sets to initial state
	for (Mesh& mesh : meshes) {
		bounding_box.Enclose(mesh.GetAABB());
	}
	UpdateOBB();
}

void Model::UpdateOBB()
{
	oriented_bounding_box.SetFrom(bounding_box, matrix);
}

bool Model::LoadMeshes(const aiScene* scene)
{
	// Create vbo using mVertices, mTextureCoords and mNumVertices
	textures.reserve(scene->mNumMeshes);
	LOG("Loading %d meshes", scene->mNumMeshes)
	for (unsigned i = 0; i < scene->mNumMeshes; i++)
	{
		LOG("Loaded mesh %d", i);
		Mesh mesh = Mesh();
		mesh.Load(scene->mMeshes[i]);
		if (mesh.IsLoaded()) {
			meshes.push_back(mesh);
		}
		else {
			LOG("Error loading mesh %d: %s", i, aiGetErrorString());
			return false;
		}			
	}
	return true;
}

void Model::CleanUp()
{
	
	for (Mesh& mesh : meshes) {
		mesh.CleanUp();
	}
	meshes.clear();
	for (Texture texture : textures) {
		glDeleteTextures(1, &texture.id);
	}
	textures.clear();
	loaded = false;
	LOG("Model %s unloaded", info.file_name.c_str())
}

void Model::UpdateMatrix()
{
	Quat rot = Quat::FromEulerXYZ(rotation.x * to_rad, rotation.y * to_rad, rotation.z * to_rad).Normalized();
	matrix = float4x4::FromTRS(position, rot, scale);
	UpdateOBB();
}

const float3 Model::GetCenter() const
{
	return oriented_bounding_box.CenterPoint();
}

const float3 Model::GetPosition() const
{
	return matrix.TranslatePart();
}

void Model::OptionsMenu()
{
	static bool locked_scale = true;
	bool changed = false; // Dirty flag

	ImGui::Text("Translation");
	if (ImGui::SliderFloat3("t.XYZ", &position[0], -5.0f, 5.0f))
		changed = true;

	ImGui::Separator();
	ImGui::Text("Scale");
	ImGui::Checkbox("Lock", &locked_scale);
	ImGui::SameLine();

	float3 scale_delta = scale;
	if (ImGui::SliderFloat3("s.XYZ", &scale[0], 0.005f, 5.0f)) {		
		if (locked_scale) {
			scale_delta -= scale;
			for (int i = 0; i < 3; i++) {
				if (scale_delta[i] != 0.0f) {
					scale = float3(scale[i]);
					break; // Only one axis can change
				}
			}
		}	
		changed = true;
	}

	ImGui::Separator();
	ImGui::Text("Rotation");
	if (ImGui::SliderFloat3("r.XYZ", &rotation[0], 0.0f, 360.0f))
		changed = true;

	if (changed)
		UpdateMatrix();	

	if (ImGui::Button("Unload"))
		CleanUp();
}

void Model::PropertiesWindow(bool* p_open)
{	
	static const ImVec4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
	static bool first_run = true;

	if (first_run) {
		ImVec2 window_size = ImGui::GetMainViewport()->Size;
		ImVec2 initial_size = ImVec2(window_size.x * 0.15f, window_size.y * 0.60f);
		ImVec2 initial_pos = ImVec2(0.0f, window_size.y * 0.025f);
		ImGui::SetNextWindowSize(initial_size, ImGuiCond_Once);
		ImGui::SetNextWindowPos(initial_pos, ImGuiCond_Once);
		first_run = false;
	}

	ImGui::Begin("Model Properties", p_open); // TODO: Fill with filename
	if (!loaded) {
		ImGui::Text("No model loaded");
		ImGui::End();
		return;
	}

	ImGui::TextColored(yellow, "Model");
	ImGui::Text("Name: %s", info.name.c_str());
	ImGui::Text("Path: %s", (info.path + info.file_name).c_str());
	ImGui::Separator();
	ImGui::TextColored(yellow, "Textures");
	for (int i = 0; i < textures.size(); ++i) {
		const Texture& texture = textures[i];
		ImGui::Text("T[%d]: %dx%d %s", i, texture.width, texture.height, texture.path.c_str());
		ImGui::Image((void*)(intptr_t)texture.id, ImVec2(150, 150));
	}

	ImGui::Separator();
	ImGui::TextColored(yellow, "Meshes");
	int vertices = 0;
	int indexes = 0;
	for (int i = 0; i < meshes.size(); ++i) {
		const Mesh& mesh = meshes[i];
		ImGui::Text("M[%d]: %dt, (%dv, %di)", i, mesh.GetNumIndexes()/3, mesh.GetNumVertices(), mesh.GetNumIndexes());
		indexes += mesh.GetNumIndexes();
		vertices += mesh.GetNumVertices();
	}
	ImGui::Text("Total : %dt, (%dv, %di)", indexes/3, vertices, indexes);

	ImGui::Separator();
	ImGui::TextColored(yellow, "Transform");
	for (int r = 0; r < 4; ++r) {
		auto row = matrix.Row(r);
		ImGui::Text("%.2f, %.2f, %.2f, %.2f", row[0], row[1], row[2], row[3]);
	}
	ImGui::End();
}
