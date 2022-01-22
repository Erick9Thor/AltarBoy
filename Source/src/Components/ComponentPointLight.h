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
	bool IsActive() const { return active; }
	void SetActive(bool v) { active = v; }

	void DrawGui() override;

	float3 GetPosition();

	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float intensity = 1.0f;
	float radius = 250.0f;

private:
	bool active = true;
};
