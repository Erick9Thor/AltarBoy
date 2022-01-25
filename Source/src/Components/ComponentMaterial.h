#pragma once

#include "Component.h"
#include "../Modules/ModuleTexture.h"
#include "../Resources/ResourceMaterial.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* conatiner);
	~ComponentMaterial() override;
	static inline Type GetType() { return Type::Material; };

	void SetMaterial(ResourceMaterial* new_material);
	const ResourceMaterial* GetMaterial() const { return material; }

	void DrawGui() override;

	void Save(JsonFormaterValue j_component) const override;
	void Load(JsonFormaterValue j_component) override;

	bool use_diffuse_texture = false;
	bool use_specular_texture = false;	

private:
	ResourceMaterial* material;	
};
