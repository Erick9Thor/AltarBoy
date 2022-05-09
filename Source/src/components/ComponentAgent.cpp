#include "core/hepch.h"
#include "ComponentAgent.h"


Hachiko::ComponentAgent::ComponentAgent(GameObject* container) : Component(Type::AGENT, container)
{
}

Hachiko::ComponentAgent::~ComponentAgent()
{
    if (agent_id != -1)
    {
        RemoveFromCrowd();
    }    
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
}

void Hachiko::ComponentAgent::Update()
{
    // Is Game running?
    // ...

    if (agent_id == -1)
    {
        // No agent no update
        return;
    }
    // Move agent through NavMesh
    const dtCrowdAgent* dt_agent = App->navigation->GetCrowd()->getAgent(agent_id);
    ComponentTransform* transform = game_object->GetTransform();
    transform->SetGlobalPosition(float3(dt_agent->npos));
}

void DebugAgentInfo(const dtCrowdAgent* ag)
{
    ImGui::Text("Debug info");
    if (!ag)
    {
        ImGui::Text("No agent in navmesh");
        return;
    }
    ImGui::Text("Active %d", ag->active);
    ImGui::Text("State %d", ag->state);
    ImGui::Text("Parial Path %d", ag->partial);
    ImGui::Text("N neighbors %d", ag->nneis);
    ImGui::Text("Desired Speed %d", ag->desiredSpeed);
    ImGui::Text("Current Pos %.2f, %.2f, %.2f", ag->npos[0], ag->npos[1], ag->npos[2]);
    ImGui::Text("Displacement %.2f, %.2f, %.2f", ag->disp[0], ag->disp[1], ag->disp[2]);
    ImGui::Text("Desired Velocity %.2f, %.2f, %.2f", ag->dvel[0], ag->dvel[1], ag->dvel[2]);
    ImGui::Text("Obstacle Adjusted Velocity %.2f, %.2f, %.2f", ag->nvel[0], ag->nvel[1], ag->nvel[2]);
    ImGui::Text("Acc Constrained Velocity %.2f, %.2f, %.2f", ag->vel[0], ag->vel[1], ag->vel[2]);
    ImGui::Separator();
    ImGui::Text("Target Info");
    ImGui::Text("Target State %d", ag->targetState);
    ImGui::Text("Target targetRef %d", ag->targetState);
    ImGui::Text("Target Pos %.2f, %.2f, %.2f", ag->targetPos[0], ag->targetPos[1], ag->targetPos[2]);
    ImGui::Text("Replaning %d", ag->targetReplan);
    ImGui::Text("Replan Time %.5f", ag->targetReplanTime);


}

void Hachiko::ComponentAgent::SetTargetPosition(const float3& target_pos)
{
    if (agent_id == -1)  return;

    ResourceNavMesh* navMesh = App->navigation->GetNavMesh();
    dtNavMeshQuery* navQuery = navMesh->GetQuery();
    dtCrowd* crowd = navMesh->GetCrowd();
    const dtQueryFilter* filter = crowd->getFilter(0);

    if (use_pathfinder)
    {
        float3 corrected_target = target_pos;
        navQuery->findNearestPoly(target_pos.ptr(), closest_point_half_range.ptr(), filter, &target_poly, corrected_target.ptr());
        assert(target_poly != 0);
        target_position = corrected_target;
        const dtCrowdAgent* ag = crowd->getAgent(agent_id);
        if (ag && ag->active)
        {
            crowd->requestMoveTarget(agent_id, target_poly, target_position.ptr());
        }
    }
    else
    {
        const dtCrowdAgent* ag = crowd->getAgent(agent_id);
        if (ag && ag->active)
        {
            float3 new_dir = (target_pos - float3(ag->npos)).Normalized();
            float3 new_vel = new_dir * max_speed;
            crowd->requestMoveVelocity(agent_id, new_vel.ptr());
        }
    }
}

void Hachiko::ComponentAgent::AddToCrowd()
{
    ResourceNavMesh* navMesh = App->navigation->GetNavMesh();
    if (agent_id != -1)
    {
        HE_LOG("Error: Tried to add an agent that was already in crowd");
        return;
    }

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
    agent_id = navMesh->GetCrowd()->addAgent(game_object->GetTransform()->GetGlobalMatrix().TranslatePart().ptr(), &ap);
}

void Hachiko::ComponentAgent::RemoveFromCrowd()
{
    if (agent_id < 0)
    {
        HE_LOG("Error: Tried to remove an agent that was not in crowd");
        return;
    }

    ResourceNavMesh* navMesh = App->navigation->GetNavMesh();
    if (!navMesh)    return;

    navMesh->GetCrowd()->removeAgent(agent_id);
    agent_id = -1;
}

void Hachiko::ComponentAgent::MoveToNearestNavmeshPoint()
{ 
    // Only callable when agent is not in navmesh at the moment, we can add logic to refresh its navmesh status later
    assert(agent_id == -1);

    
    ResourceNavMesh* navMesh = App->navigation->GetNavMesh();
    dtCrowd* crowd = navMesh->GetCrowd();
    dtNavMeshQuery* navQuery = navMesh->GetQuery();
    
    ComponentTransform* transform = game_object->GetTransform();
    
    const dtQueryFilter* filter = crowd->getFilter(0);

    float3 corrected_position = transform->GetGlobalPosition();
    navQuery->findNearestPoly(transform->GetGlobalPosition().ptr(), closest_point_half_range.ptr(), filter, &target_poly, corrected_position.ptr());
    if (target_poly != 0)
    {
        transform->SetGlobalPosition(corrected_position);
    }    
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

    if (agent_id != -1 && ImGui::Button("Remove From Navmesh"))
    {
        RemoveFromCrowd();
    }
    if (agent_id == -1 && ImGui::Button("Move To Nearest Navmesh Point"))
    {
        MoveToNearestNavmeshPoint();
    }
    if (agent_id == -1 && ImGui::Button("Add To Namesh"))
    {
        AddToCrowd();
    }
    // TODO: Delete before VS2 this shit is just to test
    ImGui::DragFloat3("newTarget", target_position.ptr(), 1.0f, -inf, inf);
    if (ImGui::Button("Feed position")) {
        SetTargetPosition(target_position);
    }

    
    DebugAgentInfo(App->navigation->GetCrowd()->getAgent(agent_id));
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