#pragma once

#include "Component.h"

#include "MathGeoLib.h"

class ComponentSpotLight : public Component
{
public:
	// TODO: add more light types
	ComponentSpotLight(GameObject* conatiner);
	~ComponentSpotLight() override;
	static inline Type GetType() { return Type::SPOTLIGHT; };

	void DebugDraw() override;

	bool IsActive() const { return active; }
	void SetActive(bool is_active) { active = is_active; }

	float3 GetPosition();
	float3 GetDirection();

	void Save(JsonFormaterValue j_component) const override;
	void Load(JsonFormaterValue j_component) override;

	void DrawGui() override;

	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float inner = 15.f;
	float outer = 30.f;
	float intensity = 1.0f;
	float radius = 250.0f;

private:
	bool active = true;
	bool draw_cone = false;
};
