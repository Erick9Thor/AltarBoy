#pragma once

#include "Component.h"

#include "MathGeoLib.h"

class ComponentPointLight : public Component
{
public:
	// TODO: add more light types
	ComponentPointLight(GameObject* conatiner);
	~ComponentPointLight() override;
	static inline Type GetType() { return Type::PointLight; };

	void DebugDraw() override;

	bool IsActive() const { return active; }
	void SetActive(bool v) { active = v; }

	void DrawGui() override;

	float3 GetPosition();

	void Save(JsonFormaterValue j_component) const override;
	void Load(JsonFormaterValue j_component) override;

	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float intensity = 1.0f;
	float radius = 250.0f;

private:
	bool active = true;
	bool draw_sphere = false;
};
