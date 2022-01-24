#pragma once
#include "Resource.h"

#include "../Modules/ModuleTexture.h"
#include "MathGeoLib.h"
#include "assimp/scene.h"

class ResourceMaterial : public Resource
{
public:
	ResourceMaterial(UID uid);
	~ResourceMaterial() override;

	bool Import(const aiMaterial* assimp_material, const std::string& model_path, const std::string& model_name);


	Texture diffuse;
	Texture specular;
	float4 diffuse_color = float4::zero;
	float4 specular_color = float4::zero;
};
