#include "core/hepch.h"
#include "utils/RandomUtilDefinitions.h"
#include "utils/RandomUtil.h"

constexpr float MapToRange(float value, float min_old, float max_old, 
    float min_new, float max_new) 
{
    float ratio = (max_new - min_new) / (max_old - min_old);
    float l = max_new - ratio * max_old;

    return ratio * value + l;
}

float Hachiko::RandomUtil::Random()
{
    return u_dist(generator);
}

float Hachiko::RandomUtil::RandomSigned()
{
    return s_dist(generator);
}

float Hachiko::RandomUtil::RandomBetween(const float a, const float b)
{
    float min = a;
    float max = b;

    if(min > max)
    {
       std::swap(min, max); 
    }

    const std::uniform_real_distribution between_dist(min, max);
    return between_dist(generator);
}

int Hachiko::RandomUtil::RandomIntBetween(int a, int b)
{
    int min = a;
    int max = b;

    if (min > max)
    {
        std::swap(min, max);
    }

    const std::uniform_int_distribution between_dist(min, max);
    return between_dist(generator);
}

float Hachiko::RandomUtil::RandomBetween(const float2& values)
{
    return RandomBetween(values.x, values.y);
}

int Hachiko::RandomUtil::RandomInt() 
{
    return std::rand() & 1 ? 1 : -1;
}

Hachiko::PerlinNoise::PerlinNoise()
{
    _seed = FloorInt(RandomUtil::RandomBetween(FLT_MIN, FLT_MAX));

    Initialize();
}

Hachiko::PerlinNoise::PerlinNoise(unsigned int seed) : _seed(seed)
{
    Initialize();
}

float Hachiko::PerlinNoise::Random(float previous)
{
    float perlin0 = std::floor(previous);
    float perlin1 = perlin0 + 1.0f;

    float delta = previous - perlin0;

    float faded_delta = Fade(delta);

    float grad0 = Gradient(perlin0);
    float grad1 = Gradient(perlin1);

    return (1.0f - faded_delta) * grad0 * (previous - perlin0) + faded_delta * grad1 * (previous - perlin1);
}

float Hachiko::PerlinNoise::RandomRange(float previous, float min, float max)
{
    return MapToRange(Random(previous), -1.0f, 1.0f, min, max);
}

float* Hachiko::PerlinNoise::GeneratePerlinNoise2d(int x, int y, int samples)
{
    float* values = new float[x * y];

    float delta = 1.0f / samples;
    float2 p = float2::zero;
    for (int i = 0; i < x; ++i)
    {
        p.x += delta;
        p.y = 0;
        for (int j = 0; j < y; ++j)
        {
            p.y += delta;

            // Calculate lattice points.
            float2 p0 = float2(std::floor(p.x), std::floor(p.y));
            float2 p1 = p0 + float2(1.0, 0.0);
            float2 p2 = p0 + float2(0.0, 1.0);
            float2 p3 = p0 + float2(1.0, 1.0);

            // Look up gradients at lattice points.
            float2 g0 = Gradient(p0);
            float2 g1 = Gradient(p1);
            float2 g2 = Gradient(p2);
            float2 g3 = Gradient(p3);

            float t0 = p.x - p0.x;
            float fade_t0 = Fade(t0); // Used for interpolation in horizontal direction

            float t1 = p.y - p0.y;
            float fade_t1 = Fade(t1); // Used for interpolation in vertical direction.

            // Calculate dot products and interpolate
            float p0p1 = (1.0 - fade_t0) * Dot(g0, (p - p0)) + fade_t0 * Dot(g1, (p - p1)); // between upper two lattice points
            float p2p3 = (1.0 - fade_t0) * Dot(g2, (p - p2)) + fade_t0 * Dot(g3, (p - p3)); // between lower two lattice points

            // Calculate final result
            values[i * x + j] = (1.0 - fade_t1) * p0p1 + fade_t1 * p2p3;
        }
    }

    return values;
}

float Hachiko::PerlinNoise::Fade(float t)
{
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float Hachiko::PerlinNoise::Gradient(float value)
{
    int gradient = _permutations[FloorInt(value) % 256];
    return (gradient < 128 ? -1.0f : 1.0f);
}

float2 Hachiko::PerlinNoise::Gradient(float2 value)
{
    int gradient_x = _permutations[FloorInt(value.x) % 256];
    int gradient_y = _permutations[FloorInt(value.y) % 256];
    return float2((gradient_x / 128.0f) - 1, (gradient_y / 128.0f) - 1).Normalized();
}

void Hachiko::PerlinNoise::Initialize()
{
    _permutations.resize(256);

    for (int i = 0; i < 256; ++i)
    {
        _permutations[i] = i;
    }

    std::default_random_engine engine(_seed);

    std::shuffle(_permutations.begin(), _permutations.end(), engine);
}