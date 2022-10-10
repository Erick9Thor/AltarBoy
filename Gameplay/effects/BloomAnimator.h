#pragma once
#include <scripting/Script.h>

namespace Hachiko
{
    class GameObject;
    class ComponentMeshRenderer;

    namespace Scripting
    {
        class BloomAnimator : public Script
        {
            SERIALIZATION_METHODS(false)

        public:
            BloomAnimator(GameObject* game_object);
            ~BloomAnimator() override = default;

            void OnAwake() override;
            void OnUpdate() override;

            void AnimateBloomManually(
                float target_intensity, 
                float duration, 
                bool should_return_to_automatic_mode = false);
            void SetShouldAnimate(bool should_animate);

        private:
            SERIALIZE_FIELD(ComponentMeshRenderer*, _target_mesh_renderer);
            SERIALIZE_FIELD(bool, _is_automatic);
            SERIALIZE_FIELD(float, _automatic_lerp_duration);

            float _initial_intensity;
            float _target_intensity;
            float _used_lerp_duration;
            float _lerp_progress;
            bool _should_return_to_automatic_mode;
            bool _should_animate;
        };
    }
}
