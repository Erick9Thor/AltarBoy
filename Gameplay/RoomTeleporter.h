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

            void SetActive(bool v);

        private:
            void Interpolate();

        public:
            const float4 _clear_color = float4::zero;
            const float4 _opaque_color = float4(0.f, 0.f, 0.f, 1.f);
            float _fade_progress = 0.f;
            bool _transition_active = false;
            bool _changing = false;
            SERIALIZE_FIELD(bool, _touching);
            SERIALIZE_FIELD(GameObject*, _target);
            SERIALIZE_FIELD(ComponentImage*, _fade_image);
            SERIALIZE_FIELD(float, _fade_duration);
            ComponentTransform2D* _image_transform = nullptr;
        private:
        };
} // namespace Scripting
} // namespace Hachiko*/
