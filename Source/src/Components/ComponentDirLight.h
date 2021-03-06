#pragma once

#include "Component.h"

#include "MathGeoLib.h"

class ComponentDirLight : public Component
{
public:
	// TODO: add more light types
	ComponentDirLight(GameObject* conatiner);
	~ComponentDirLight() override;
	static inline Type GetType() { return Type::DIRLIGHT; };

	void DebugDraw() override;

	bool IsActive() const { return active; }
	void SetActive(bool is_active) { active = is_active; }

	float3 GetDirection() const;

	void Save(JsonFormaterValue j_component) const override;
	void Load(JsonFormaterValue j_component) override;

	void DrawGui() override;

	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float intensity = 1.0f;

private:
	bool active = true;
	bool draw_direction = false;
};
