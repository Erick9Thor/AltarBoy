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

    virtual void Start() {};
    virtual void Update() {};
    virtual void Disable() {};
    virtual void Enable() {};
    void DrawGui() override;

    const std::string& GetName() const;

protected:
    explicit Script(GameObject* new_game_object, std::string new_name);

private:
    std::string name;

};
} // mamespace Scripting
} // namespace Hachiko