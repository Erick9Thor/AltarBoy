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

	void DrawGui() override;

	float3 GetPosition();

	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
};
