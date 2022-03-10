#include "core/hepch.h"

#include "components/ComponentTransform.h"
#include "components/ComponentCamera.h"
#include "components/ComponentMesh.h"
#include "components/ComponentMaterial.h"
#include "components/ComponentDirLight.h"
#include "components/ComponentPointLight.h"
#include "components/ComponentSpotLight.h"

#include <debugdraw.h>

Hachiko::GameObject::GameObject(const char* name) :
    name(name)
{
    AddComponent(new ComponentTransform(this, float3::zero, Quat::identity, float3::one));
}

Hachiko::GameObject::GameObject(GameObject* parent, const float4x4& transform, const char* name, UID uid) :
    name(name),
    uid(uid)
{
    SetNewParent(parent);
    AddComponent(new ComponentTransform(this, transform));
}

Hachiko::GameObject::GameObject(GameObject* parent, const char* name, UID uid, const float3& translation, const Quat& rotation, const float3& scale) :
    name(name),
    uid(uid)
{
    SetNewParent(parent);
    AddComponent(new ComponentTransform(this, translation, rotation, scale));
}

Hachiko::GameObject::~GameObject()
{
    if (parent)
    {
        parent->RemoveChild(this);
    }

    if (scene_owner)
        scene_owner->DestroyGameObject(this);

    for (GameObject* child : children)
    {
        child->parent = nullptr;
        RELEASE(child);
    }
    for (Component* component : components)
    {
        RELEASE(component);
    }
}

void Hachiko::GameObject::RemoveChild(GameObject* game_object)
{
    children.erase(std::remove(children.begin(), children.end(), game_object), children.end());
}

void Hachiko::GameObject::SetNewParent(GameObject* new_parent)
{
    if (new_parent == parent)
    {
        return;
    }

    if (parent)
    {
        parent->RemoveChild(this);
    }

    if (new_parent)
    {
        new_parent->children.push_back(this);
    }
    parent = new_parent;
}

void Hachiko::GameObject::AddComponent(Component* component)
{
    switch (component->GetType())
    {
    case (Component::Type::TRANSFORM):
    {
        components.push_back(component);
        transform = static_cast<ComponentTransform*>(component);
        component->SetGameObject(this);
        break;
    }
    case (Component::Type::CAMERA):
    {
        components.push_back(component);
        component->SetGameObject(this);
        break;
    }
    }
}

Hachiko::Component* Hachiko::GameObject::CreateComponent(Component::Type type)
{
    Component* new_component = nullptr;
    switch (type)
    {
    case (Component::Type::TRANSFORM):
        return transform;
        break;
    case (Component::Type::CAMERA):
        new_component = new ComponentCamera(this);
        break;
    case (Component::Type::MESH):
        new_component = new ComponentMesh(this);
        break;
    case (Component::Type::MATERIAL):
        new_component = new ComponentMaterial(this);
        break;
    case (Component::Type::DIRLIGHT):
        new_component = new ComponentDirLight(this);
        break;
    case (Component::Type::POINTLIGHT):
        new_component = new ComponentPointLight(this);
        break;
    case (Component::Type::SPOTLIGHT):
        new_component = new ComponentSpotLight(this);
        break;
    }

    if (new_component != nullptr)
    {
        components.push_back(new_component);
    }
    else
    {
        HE_LOG("Falied to create component");
    }
    return new_component;
}

void Hachiko::GameObject::Update()
{
    if (transform->HasChanged())
    {
        OnTransformUpdated();
    }

    for (Component* component : components)
    {
        component->Update();
    }

    for (GameObject* child : children)
    {
        if (child->IsActive())
        {
            child->Update();
        }
    }
}

void Hachiko::GameObject::DrawAll(ComponentCamera* camera, Program* program) const
{
    // Draw yourself
    Draw(camera, program);
    // Draw children recursively
    for (const GameObject* child : children)
    {
        child->DrawAll(camera, program);
    }
}

void Hachiko::GameObject::Draw(ComponentCamera* camera, Program* program) const
{
    // Call draw on all components
    for (Component* component : components)
    {
        component->Draw(camera, program);
    }
}

void Hachiko::GameObject::DrawStencil(ComponentCamera* camera, Program* program)
{
    auto* mesh = GetComponent<ComponentMesh>();
    if (mesh)
    {
        mesh->DrawStencil(camera, program);
    }
}

void Hachiko::GameObject::OnTransformUpdated()
{
    // Update components
    for (Component* component : components)
    {
        component->OnTransformUpdated();
    }

    // Update children
    for (GameObject* child : children)
    {
        child->OnTransformUpdated();
    }

    UpdateBoundingBoxes();
}

void Hachiko::GameObject::DebugDrawAll()
{
    // Draw yourself
    DebugDraw();
    // Draw children recursively
    for (GameObject* child : children)
    {
        child->DebugDrawAll();
    }
}

void Hachiko::GameObject::DebugDraw() const
{
    DrawBoundingBox();
    for (Component* component : components)
    {
        component->DebugDraw();
    }
}

void Hachiko::GameObject::DrawBoundingBox() const
{
    ddVec3 p[8];
    // This order was pure trial and error, i dont know how to really do it
    // Using center and points does not show the rotation
    static const int order[8] = {0, 1, 5, 4, 2, 3, 7, 6};
    for (int i = 0; i < 8; ++i)
    {
        p[i] = obb.CornerPoint(order[i]);
    }
    dd::box(p, dd::colors::White);
}

void Hachiko::GameObject::UpdateBoundingBoxes()
{
    auto* mesh = GetComponent<ComponentMesh>();
    if (mesh != nullptr)
    {
        obb = mesh->GetAABB();
        obb.Transform(transform->GetMatrix());
        // Enclose is accumulative, reset the box
        aabb.SetNegativeInfinity();
        aabb.Enclose(obb);
    }
    else
    {
        constexpr float default_bounding_size = 1.0f;
        // If there is no mesh generate a default size
        aabb.SetNegativeInfinity();
        aabb.SetFromCenterAndSize(transform->GetPosition(), float3(default_bounding_size));
        obb = aabb;
    }

    // Without the check main camera crashes bcs there is no quadtree
    if (scene_owner)
    {
        const Quadtree* quadtree = scene_owner->GetQuadtree();
        quadtree->Remove(this);
        quadtree->Insert(this);
    }
}

void Hachiko::GameObject::RemoveComponent(Component* component)
{
    //TODO: Should I delete the component?
    components.erase(std::remove(components.begin(), components.end(), component));
}

void Hachiko::GameObject::Save(JsonFormatterValue j_gameObject) const
{
    j_gameObject["Uid"] = uid;
    j_gameObject["GOName"] = name.c_str();
    j_gameObject["Active"] = active;
    j_gameObject["ParentId"] = parent != nullptr ? parent->uid : 0;

    const JsonFormatterValue j_components = j_gameObject["Components"];
    for (unsigned i = 0; i < components.size(); ++i)
    {
        JsonFormatterValue j_component = j_components[i];
        const Component* component = components[i];

        j_component["ComponentID"] = component->GetID();
        j_component["ComponentType"] = static_cast<int>(component->GetType());
        component->Save(j_component);
    }

    const JsonFormatterValue j_children = j_gameObject["GOChildrens"];
    for (unsigned i = 0; i < children.size(); ++i)
    {
        const JsonFormatterValue j_child = j_children[i];
        const GameObject* child = children[i];
        child->Save(j_child);
    }
}

void Hachiko::GameObject::Load(JsonFormatterValue j_gameObject)
{
    const JsonFormatterValue j_components = j_gameObject["Components"];
    for (unsigned i = 0; i < j_components.Size(); ++i)
    {
        JsonFormatterValue j_component = j_components[i];

        UID c_uid = j_component["ComponentID"];
        int enum_type = j_component["ComponentType"];
        bool active = j_component["Active"];

        const auto type = static_cast<Component::Type>(enum_type);

        Component* component = CreateComponent(type);

        component->Load(j_component);
    }

    const JsonFormatterValue j_children = j_gameObject["GOChildrens"];
    for (unsigned i = 0; i < j_children.Size(); ++i)
    {
        JsonFormatterValue j_child = j_children[i];

        std::string child_name = j_child["GOName"];
        const auto child = new GameObject(this, child_name.c_str(), j_child["Uid"]);
        child->scene_owner = scene_owner;
        child->Load(j_child);
    }
}
