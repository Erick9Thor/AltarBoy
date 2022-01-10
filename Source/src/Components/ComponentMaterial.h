#pragma once

#include "Component.h"
#include "../Modules/ModuleTexture.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* conatiner);
	~ComponentMaterial() override;
	static inline Type GetType() { return Type::Material; };

	void SetTexture(Texture new_texture) { texture = new_texture; }
	inline unsigned GetTextureId() const { return texture.id; }

	void DrawGui() override;

	void Save(JsonFormaterValue jComponent) const override;
	void Load(JsonFormaterValue jComponent) override;

private:
	Texture texture;
};
