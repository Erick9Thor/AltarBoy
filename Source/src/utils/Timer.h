#pragma once

constexpr float ONE_SEC_IN_MS = 1000.0f;

namespace Hachiko
{
    class Timer
    {
    public:
        void Start();
        void Resume();
        double Read();
        double Stop();

        [[nodiscard]] bool IsRunning() const
        {
            return running;
        }

    private:
        bool running = false;
        unsigned long long start_time{};
        unsigned long long stop_time{};
        double current_time{};
    };

    class PerformanceTimer
    {
    public:
        void Start();
        void Resume();
        double Read();
        double Stop();

        [[nodiscard]] bool IsRunning() const
        {
            return running;
        }

    private:
        bool running = false;
        unsigned long long start_time{};
        unsigned long long stop_time{};
        double current_time{};
    };

    class GameTimer
    {
    public:
        static void Start();
        static double Update();

        static void Play();
        static void Pause();
        static void Resume();
        static void Stop();

        static PerformanceTimer timer;

        static double delta_time;
        static double total_time;
        static double prev_tick_time;

        static bool running;
        static bool paused;
    };

    class EngineTimer
    {
    public:
        static void Start();
        static double Update();

        static PerformanceTimer timer;
        static double delta_time;
        static double total_time;
        static double prev_tick_time;
        static bool running;

        static unsigned fps;
    private:
        static double last_time_read;
        static unsigned fps_count;
        static void CountFPS();
    };
}
