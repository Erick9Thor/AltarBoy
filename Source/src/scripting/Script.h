#pragma once

#include "HachikoApiDefine.h"
#include "scripting/ScriptingMacros.h"
#include "scripting/serialization/ISerializable.h"
#include "components/Component.h"

#include <unordered_map>

namespace Hachiko
{
class GameObject;

namespace Scripting
{
class HACHIKO_API Script : public Component, public ISerializable
{
    SERIALIZATION_METHODS(true)
public:
    virtual ~Script() = default;
    
    void Update() override;

    virtual void OnAwake() {};
    virtual void OnStart() {};
    virtual void OnUpdate() {};
    virtual void OnDisable() {};
    virtual void OnEnable() {};
    void DrawGui() override;

    const std::string& GetName() const;

protected:
    explicit Script(GameObject* new_game_object, std::string new_name);

private:
    std::string name;

};
} // mamespace Scripting
} // namespace Hachiko