#pragma once

#include <scripting/Script.h>

namespace Hachiko
{
    class GameObject;
    class ComponentTransform;
    class ComponentImage;
    namespace Scripting
    {
        class RoomTeleporter : public Script
        {
            SERIALIZATION_METHODS(false)

        public:
            RoomTeleporter(GameObject* game_object);
            ~RoomTeleporter() override = default;

            void OnAwake() override;
            void OnStart() override;
            void OnUpdate() override;

        private:

        public:
            SERIALIZE_FIELD(bool, _touching);
            SERIALIZE_FIELD(GameObject*, _target);
            SERIALIZE_FIELD(ComponentImage*, _fade_image);

        private:
        };
} // namespace Scripting
} // namespace Hachiko*/
