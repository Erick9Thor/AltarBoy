#pragma once

#include "Component.h"
#include "../Modules/ModuleTexture.h"
#include "../Scene.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* conatiner);
	~ComponentMaterial() override;
	static inline Type GetType() { return Type::Material; };

	void SetMaterial(Scene::Material new_material) { material = new_material; }
	inline unsigned GetTextureId() const { return material.diffuse.id; }

	void DrawGui() override;

	void Save(JsonFormaterValue j_component) const override;
	void Load(JsonFormaterValue j_component) override;

private:
	Scene::Material material;
};
