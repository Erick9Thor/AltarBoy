#pragma once

#include "Component.h"

#include "MathGeoLib.h"

class ComponentDirLight : public Component
{
public:
	// TODO: add more light types
	ComponentDirLight(GameObject* conatiner);
	~ComponentDirLight() override;
	static inline Type GetType() { return Type::DirLight; };

	void DebugDraw() override;

	bool IsActive() const { return active; }
	void SetActive(bool v) { active = v; }

	void DrawGui() override;

	float3 GetDirection() const;

	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float intensity = 1.0f;

private:
	bool active = true;
	bool draw_direction = false;
};
