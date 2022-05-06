#pragma once

#include "Application.h"
#include "modules/ModuleSceneManager.h"
#include "modules/ModuleNavigation.h"
#include "modules/ModuleEvent.h"

#include "resources/ResourceNavMesh.h"

#include "components/ComponentTransform.h"

#include "DetourCrowd.h"

namespace Hachiko
{
	class ComponentAgent : public Component
	{

	public:
		ComponentAgent(GameObject* container);
		~ComponentAgent() = default;

		void Start() override;
		void Update() override;

		static Type GetType()
        {
            return Type::AGENT;
        }

		void SetTargetPosition(const float3& target_pos);
		void SetMaxSpeed(float new_max_speed);
		void SetMaxAcceleration(float new_max_speed);
		void SetObstacleAvoidance(bool obstacle_avoidance);

		float GetMaxSpeed() const { return max_speed; }
		float GetMaxAcceleration() const { return max_acceleration; }
		bool IsAvoidingObstacles() const { return avoid_obstacles; }

		void AddToCrowd();
		void RemoveFromCrowd();
		
		void Save(YAML::Node& node) const override;
		void Load(const YAML::Node& node) override;

		void DrawGui() override;
	private:
        float3 target_position = float3::zero;
		unsigned int target_poly = 0;
		int agent_id = -1;

		float max_speed = 5.0f;
		float max_acceleration = 2.0f;
		bool avoid_obstacles = true;
		bool use_pathfinder = true;

	};
}


