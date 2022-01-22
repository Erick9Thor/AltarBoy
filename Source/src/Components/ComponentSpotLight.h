#pragma once

#include "Component.h"

#include "MathGeoLib.h"

class ComponentSpotLight : public Component
{
public:
	// TODO: add more light types
	ComponentSpotLight(GameObject* conatiner);
	~ComponentSpotLight() override;
	static inline Type GetType() { return Type::SpotLight; };
	bool IsActive() const { return active; }
	void SetActive(bool v) { active = v; }

	void DrawGui() override;

	float3 GetPosition();
	float3 GetDirection();

	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float inner = 1.f;
	float outer = 1.2f;

private:
	bool active = true;
};
