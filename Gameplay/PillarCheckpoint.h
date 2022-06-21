#pragma once

#include <scripting/Script.h>

namespace Hachiko
{
    class GameObject;
    class ComponentTransform;
    class ComponentImage;
    namespace Scripting
    {
        class PillarCheckpoint : public Script
        {
            SERIALIZATION_METHODS(false)

        public:
            PillarCheckpoint(GameObject* game_object);
            ~PillarCheckpoint() override = default;

            void OnAwake() override;
            void OnStart() override;
            void OnUpdate() override;


            bool IsActive() { return _is_active; }
        private:
            bool IsPlayerInRange();
        public:
            SERIALIZE_FIELD(GameObject*, _player);
        private:
            SERIALIZE_FIELD(float3, _restart_position);
            SERIALIZE_FIELD(float, _activation_range);

            ComponentAnimation* _animation;
            float3 _player_pos;
            bool _is_active;
        };
    } // namespace Scripting
} // namespace Hachiko*/
