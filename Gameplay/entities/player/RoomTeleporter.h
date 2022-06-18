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

            enum Step
            {
                WAITING = 0,
                FADE_IN = 1,
                BLACKOUT = 2,
                FADE_OUT = 3,
            };

        public:
            RoomTeleporter(GameObject* game_object);
            ~RoomTeleporter() override = default;

            void OnAwake() override;
            void OnStart() override;
            void OnUpdate() override;

        private:
            void SetActive(bool v);
            void EnterRoom();
            void ExitRoom();
            void SimulateTransition();
            void TeleportPlayer(bool indoors);

        public:
            const float4 _clear_color = float4::zero;
            const float4 _opaque_color = float4(0.f, 0.f, 0.f, 1.f);
            float _step_progress = 0.f;
            Step _step = Step::WAITING;
            bool _inside_room = false;
            bool _already_triggered = false;
            SERIALIZE_FIELD(GameObject*, _player);
            SERIALIZE_FIELD(GameObject*, _room_portal);
            SERIALIZE_FIELD(GameObject*, _outdoor_portal);
            SERIALIZE_FIELD(float, _trigger_distance);
            SERIALIZE_FIELD(ComponentImage*, _fade_image);
            SERIALIZE_FIELD(float, _fade_duration);
            SERIALIZE_FIELD(float, _blackout_duration);
            ComponentTransform2D* _image_transform = nullptr;
        private:
        };
} // namespace Scripting
} // namespace Hachiko*/
