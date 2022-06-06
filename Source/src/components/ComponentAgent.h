#pragma once


namespace Hachiko
{
    class HACHIKO_API ComponentAgent : public Component
	{

	public:
		ComponentAgent(GameObject* container);
		~ComponentAgent() override;

		void Update() override;
        void Start() override;
        void Stop() override;
		
		void SetTargetPosition(const float3& target_pos);
		void SetMaxSpeed(float new_max_speed);
		void SetMaxAcceleration(float new_max_acceleration);
		void SetObstacleAvoidance(bool obstacle_avoidance);

		float GetMaxSpeed() const { return max_speed; }
		float GetMaxAcceleration() const { return max_acceleration; }
		bool IsAvoidingObstacles() const { return avoid_obstacles; }

		void AddToCrowd();
		void RemoveFromCrowd();

		void MoveToNearestNavmeshPoint();		
		
		void Save(YAML::Node& node) const override;
		void Load(const YAML::Node& node) override;

		void DrawGui() override;
	private:
        float3 closest_point_half_range = float3(10.f, 10.f, 10.f);
        float3 target_position = float3::zero;
		unsigned int target_poly = 0;
		int agent_id = -1;

		float max_speed = 5.0f;
		float max_acceleration = 2.0f;
		bool avoid_obstacles = false;
		bool use_pathfinder = true;

		bool show_debug_info = false;
	};
}


