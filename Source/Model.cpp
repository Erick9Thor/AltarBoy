#include "Globals.h"
#include "Model.h"
#include "Application.h"

#include "ModuleTexture.h"

#include "glew.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

#include "il.h"
#include "ilu.h"

Model::Model(const string& model_path)
{
	Assimp::Importer importer;

	path = model_path.substr(0, model_path.find_last_of("/\\") + 1);

	if (path.size() > 0) {
		LOG("[Model] Loading model from this path: %s", path.c_str());
	}

	file_name = model_path.substr(model_path.find_last_of("/\\") + 1);
	name = file_name.substr(0, std::string::size_type(file_name.find_last_of('.')));

	LOG("[Model] Loading model: %s", name.c_str());

	m_model = float4x4::identity;
	local_pos = float3(0.0f, 0.0f, 0.0f);
	local_rot_euler = float3(0.0f, 0.0f, 0.0f);
	local_scale = float3(1.0f, 1.0f, 1.0f);
	m_rotation = Quat::identity;

	const aiScene* scene = importer.ReadFile(model_path, aiProcess_Triangulate);
	if (scene)
	{
		LoadTextures(scene);
		LoadMeshes(scene);

		for (unsigned int i = 0; i < meshes.size(); ++i)
		{
			num_triangles += meshes[i]->GetNumIndices() / 3;
			num_vertices += meshes[i]->GetNumVertices();
		}
	}
	else
	{
		LOG("[Model] Error loading %s: %s", model_path, aiGetErrorString());
	}
}

Model::~Model()
{
	for (int i = 0; i < textures.size(); ++i)
		delete textures[i];
	for (int i = 0; i < meshes.size(); ++i)
		delete meshes[i];
}

void Model::Draw()
{
	for (unsigned int i = 0; i < meshes.size(); ++i) {
		meshes[i]->Draw(textures, m_model);
	}
}

void Model::LoadTextures(const aiScene* scene)
{
	LOG("[Model] Texture loading for model...");

	aiString file;
	textures = vector<Texture*>(scene->mNumMaterials);
	
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{		
			textures[i] = new Texture(file.data, path.c_str());
		}
		else {
			LOG("[Model] Failed loading texture %s:", aiGetErrorString());
		}
	}
}

void Model::LoadMeshes(const aiScene* scene)
{
	LOG("[Model] Loading meshes");

	m_aabb.SetNegativeInfinity();
	meshes = vector<Mesh*>(scene->mNumMeshes);

	for (unsigned i = 0; i < scene->mNumMeshes; i++)
	{
		if (scene->mMeshes[i]->HasPositions()) {
			meshes[i] = new Mesh(scene->mMeshes[i]);
			m_aabb.Enclose(meshes[i]->getAABB());
		}
		else {
			LOG("[Model] Failed loading mesh: %d: %s", i, aiGetErrorString());
		}
	}
}

void Model::DrawGui()
{
	ImGui::Indent();

	char go_name[50];
	strcpy_s(go_name, 50, name.c_str());
	ImGuiInputTextFlags name_input_flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
	ImGui::InputText("###", go_name, 50, name_input_flags);
	ImGui::SameLine();

	ImGui::Unindent();

	ImGui::Separator();

	ImGui::Text("Vertices: %i", num_vertices);
	ImGui::Text("Triangles: %i", num_triangles);

	ImGui::Separator();

	DrawTransform();
}

void Model::DrawTransform()
{
	ImGui::Indent();

	if (ImGui::Button("Reset"))
	{
		m_model = float4x4::identity;
	}

	if (ImGui::SliderFloat3("Position", &local_pos[0], -10.0f, 10.0f))
	{
		m_model = float4x4::FromTRS(local_pos, m_rotation, local_scale);
	}
	if (ImGui::SliderFloat3("Rotation", &local_rot_euler[0], 0.0f, 360.0f))
	{
		m_rotation = Quat::FromEulerXYZ(local_rot_euler.x * DEGTORAD, local_rot_euler.y * DEGTORAD, local_rot_euler.z * DEGTORAD);
		m_model = float4x4::FromTRS(local_pos, m_rotation, local_scale);
	}
	if (ImGui::SliderFloat3("Scale", &local_scale[0], 0, 10.0f))
	{
		m_model = float4x4::FromTRS(local_pos, m_rotation, local_scale);
	}

	ImGui::Separator();
	ImGui::Separator();

	ImGui::Text("Local Matrix");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			ImGui::Text("%f", m_model[j][i]);
			if (j < 3) ImGui::SameLine();
		}
	}
	ImGui::PopStyleColor();
	ImGui::Separator();
}

float3 Model::GetCenter()
{
	return m_aabb.CenterPoint();
}


