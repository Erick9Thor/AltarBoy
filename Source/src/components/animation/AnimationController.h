#pragma once

#include <Math.h>

#include <vector>

namespace Hachiko
{
    class AnimationController
    {
        struct Instance
        {
            UID clip = 0;
            unsigned time = 0;
            bool loop = true;
            float speed = 1.0;

            Instance* next = nullptr;
            unsigned fade_duration = 0;
            unsigned fade_time = 0;
        };

        Instance* current = nullptr;

        mutable std::vector<float> tmpWeights;

    public:

        AnimationController();
        ~AnimationController();

        void Play(UID clip, bool loop, unsigned fade_time);
        void Update(unsigned elapsed);
        void Stop();

        bool GetTransform(const std::string& channel_name, math::float3& position, Quat& rotation) const;

    private:
        // INTERPOLATION
        float Interpolate(float first, float second, float lambda);
        float3 Interpolate(const float3& first, const float3& second, float lambda); 
        Quat Interpolate(const Quat& first, const Quat& second, float lambda) const;

        // MANAGE INSTANCES
        void UpdateInstance(Instance* instance, unsigned elapsed);
        void ReleaseInstance(Instance* instance);
    };
} // namespace Hachiko
