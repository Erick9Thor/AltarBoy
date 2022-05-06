#include "core/hepch.h"
#include "ComponentAgent.h"


Hachiko::ComponentAgent::ComponentAgent(GameObject* container) : Component(Type::AGENT, container)
{
	target_position = float3::zero;
	target_poly = 0;

	max_speed = 5.0f;
	max_acceleration = 2.0f;
	avoid_obstacles = true;
    join_crowd = true;
    use_pathfinder = true;
    agent_id = -1;

    // TODO: Remove this call when HE lifecycle is fixed
    Start();
}

void Hachiko::ComponentAgent::Start()
{
    std::function handleGameStateChanges = [&](Event& evt)
    {
        const auto& e = evt.GetEventData<GameStateEventPayload>();
        if (e.GetState() == GameStateEventPayload::State::STARTED)
        {
            AddToCrowd();
        }
        else if (e.GetState() == GameStateEventPayload::State::STOPPED)
        {
            RemoveFromCrowd();
        }
    };
    App->event->Subscribe(Event::Type::GAME_STATE, handleGameStateChanges);
    // TODO: REMOVE. ADDED FOR DEBUG PURPOUSES
    AddToCrowd();
}

void Hachiko::ComponentAgent::Update()
{
    // Is Game running?
    // ...
        if (App->scene_manager->GetActiveScene() != GetGameObject()->scene_owner)    return;
        ResourceNavMesh* navMesh = App->navigation->GetNavMesh();
        if (!navMesh)
        {
            HE_LOG("NavMesh not built in this scene.");
            return;
        }

        // Try to add agent to crowd
        if(join_crowd)    AddToCrowd();
        if (agent_id == -1)  return;

        // Move agent through NavMesh
        const dtCrowdAgent* dt_agent = navMesh->GetCrowd()->getAgent(agent_id);
        ComponentTransform* agents_transform = GetGameObject()->GetComponent<ComponentTransform>();
        agents_transform->SetGlobalPosition(float3(dt_agent->npos));
}

void Hachiko::ComponentAgent::SetTargetPosition(const float3& target_pos)
{
    if (agent_id == -1)  return;

    ResourceNavMesh* navMesh = App->navigation->GetNavMesh();
    dtNavMeshQuery* navQuery = navMesh->GetQuery();
    dtCrowd* crowd = navMesh->GetCrowd();
    const dtQueryFilter* filter = crowd->getFilter(0);
    const float* queryExtents = crowd->getQueryExtents();

    if (use_pathfinder)
    {
        navQuery->findNearestPoly(target_pos.ptr(), queryExtents, filter, &target_poly, target_position.ptr());
        const dtCrowdAgent* ag = crowd->getAgent(agent_id);
        if (!ag) return;
        if (ag->active)
        {
            crowd->requestMoveTarget(agent_id, target_poly, target_position.ptr());
        }
    }
    else
    {
        const dtCrowdAgent* ag = crowd->getAgent(agent_id);
        if (!ag) return;
        if (ag->active)
        {
            float3 new_dir = (target_pos - float3(ag->npos)).Normalized();
            float3 new_vel = new_dir * max_speed;
            crowd->requestMoveVelocity(agent_id, new_vel.ptr());
        }
    }
}

void Hachiko::ComponentAgent::AddToCrowd()
{
    join_crowd = true;
    ResourceNavMesh* navMesh = App->navigation->GetNavMesh();
    if (agent_id != -1)  return;

    // PARAMS INIT
    dtCrowdAgentParams ap;
    memset(&ap, 0, sizeof(ap));
    ap.radius = navMesh->agent_radius;
    ap.height = navMesh->agent_height;
    ap.maxAcceleration = max_acceleration;
    ap.maxSpeed = max_speed;
    ap.collisionQueryRange = ap.radius * 12.0f;
    ap.pathOptimizationRange = ap.radius * 30.0f;
    ap.updateFlags = 0;

    ap.updateFlags |= DT_CROWD_ANTICIPATE_TURNS;
    ap.updateFlags |= DT_CROWD_OPTIMIZE_VIS;
    ap.updateFlags |= DT_CROWD_OPTIMIZE_TOPO;
    ap.updateFlags |= DT_CROWD_SEPARATION;
    if (avoid_obstacles) {
        ap.updateFlags |= DT_CROWD_OBSTACLE_AVOIDANCE;
    }

    ap.obstacleAvoidanceType = 3;
    ap.separationWeight = 2;

    // Add to Crowd
    agent_id = navMesh->GetCrowd()->addAgent(GetGameObject()->GetComponent<ComponentTransform>()->GetGlobalPosition().ptr(), &ap);

    //join_crowd = false;
}

void Hachiko::ComponentAgent::RemoveFromCrowd()
{
    join_crowd = false;

    ResourceNavMesh* navMesh = App->navigation->GetNavMesh();
    if (!navMesh)    return;

    navMesh->GetCrowd()->removeAgent(agent_id);
    agent_id = -1;
}

void Hachiko::ComponentAgent::DrawGui()
{
    ImGui::PushID(this);
    if (ImGuiUtils::CollapsingHeader(game_object, this, "Agent Component"))
    {
        ImGui::DragFloat("Max Speed", &max_speed, 1.0f, 0.0f);
        ImGui::DragFloat("Max Acceleration", &max_acceleration, 1.0f, 0.0f);
        ImGui::Checkbox("Use Pathfinding", &use_pathfinder);
        ImGui::Checkbox("Avoid obstacles (Pathfinding)", &avoid_obstacles);
    }
    // TODO: Delete before VS2 this shit is just to test
    ImGui::DragFloat3("newTarget", target_position.ptr(), 1.0f, -inf, inf);
    if (ImGui::Button("Feed position")) {
        SetTargetPosition(target_position);
    }
    ImGui::PopID();
}

void Hachiko::ComponentAgent::Save(YAML::Node& node) const
{
    node[MAX_SPEED] = max_speed;
    node[MAX_ACCELERATION] = max_acceleration;
    node[AVOID_OBSTACLES] = avoid_obstacles;
}

void Hachiko::ComponentAgent::Load(const YAML::Node& node)
{
    max_speed = node[MAX_SPEED].as<float>();
    max_acceleration = node[MAX_ACCELERATION].as<float>();
    avoid_obstacles = node[AVOID_OBSTACLES].as<bool>();
}