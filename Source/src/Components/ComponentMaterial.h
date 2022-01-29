#pragma once

#include "Component.h"
#include "../Modules/ModuleTexture.h"
#include "../Resources/ResourceMaterial.h"

#include "assimp/scene.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* conatiner);
	~ComponentMaterial() override;
	static inline Type GetType() { return Type::MATERIAL; };

	const ResourceMaterial* GetMaterial() const { return material; }

	void DrawGui() override;

	void Import(aiMaterial* assimp_material, const std::string& model_path, const std::string& model_name);
	void Save(JsonFormaterValue j_component) const override;
	void Load(JsonFormaterValue j_component) override;

	bool use_diffuse_texture = false;
	bool use_specular_texture = false;	

private:
	ResourceMaterial* material;	
};
