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
		float3 target_position;
		unsigned int target_poly;
		int agent_id;

		float max_speed;
		float max_acceleration;
		bool avoid_obstacles;
		bool join_crowd;

		bool is_game_running;
	};
}


