#pragma once

#include "ModuleTexture.h"
#include "Mesh.h"

#include "assimp/scene.h"
#include "MathGeoLib.h"

#include <vector>
#include <string>

class Model
{
public:
	Model();
	~Model();

	void Load(const std::string& file_name);
	void Draw();
	void CleanUp();

	const float3 GetCenter() const;
	const float3 GetPosition() const;
	const OBB& GetOBB() { return oriented_bounding_box; }

	void OptionsMenu();
	void PropertiesWindow(bool* p_open);

private:
	bool LoadMeshes(const aiScene* scene);
	bool LoadTextures(const aiScene* scene);
	bool LoadTexture(const aiMaterial* texture);

	void GenerateAABB();
	void UpdateOBB();

	void UpdateMatrix();

	struct ModelInfo {
		std::string name;
		std::string path;
		std::string file_name;
	};

	ModelInfo info;
	bool loaded = false;

	float4x4 matrix = float4x4::identity;

	float3 position;
	float3 rotation;
	float3 scale;

	std::vector<Texture> textures;
	std::vector<Mesh> meshes;

	AABB bounding_box;
	OBB oriented_bounding_box;
};

