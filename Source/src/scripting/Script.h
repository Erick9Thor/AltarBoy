#pragma once

#include "Globals.h"
#include "ScriptingMacros.h"
#include "scripting/serialization/SerializedField.h"
#include "scripting/serialization/ISerializable.h"
#include "components/Component.h"

#include <unordered_map>

namespace Hachiko
{
class GameObject;

namespace Scripting
{
// TODO: Make Script inherit from Component as well.
class HACHIKO_API Script : public ISerializable
{
    SERIALIZATION_METHODS(true)
public:
    virtual ~Script() = default;

    virtual void Start() {};
    virtual void Update() {};
    virtual void Disable() {};
    virtual void Enable() {};

    const std::string& GetName() const;

protected:
    explicit Script(GameObject* new_game_object, std::string new_name);

protected:
    GameObject* game_object;

private:
    std::string name;

};
} // mamespace Scripting
} // namespace Hachiko