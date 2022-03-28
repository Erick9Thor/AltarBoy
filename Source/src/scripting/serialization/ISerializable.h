#include "Globals.h"
#include "scripting/serialization/SerializedField.h"
#include <unordered_map>

namespace Hachiko
{
namespace Scripting
{
class HACHIKO_API ISerializable
{
public:
    virtual void SerializeTo (
        std::unordered_map<std::string, SerializedField>& serialized_fields
    ) = 0;

    virtual void DeserializeFrom(
        std::unordered_map<std::string, SerializedField>& serialized_fields
    ) = 0;

    virtual void ShowInEditor() {};
};
} // namespace Scripting
} // namespace Hachiko