#pragma once
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>
#pragma warning(push)
#pragma warning(disable:4146) // https://github.com/imneme/pcg-cpp/issues/11
#include "Utils/pcg_random.hpp"
#pragma warning(pop)
#include "Core/Vec3.h"

class RandomGenerator
{
public:
    RandomGenerator() = default;

    RandomGenerator(unsigned int seed);

    float nextFloat();
    Vec3 randomInUnitSphere();
    Vec3 randomInUnitDisk();
    Vec3 randomUnitVector();

    static float randFloat();

private:
    pcg32 m_engine;
    std::uniform_real_distribution<float> m_floatDistribution;

    static pcg32 s_engine;
    static std::uniform_real_distribution<float> s_floatDistribution;
};

inline float RandomGenerator::nextFloat()
{
    return m_floatDistribution(m_engine);
}

//// Initialize random engine using standard mersenne twister
//// seeded with randomDevice
//std::random_device randomDevice;
//std::mt19937 gen(randomDevice());
//std::uniform_real_distribution<float> distribution(0.f, 1.f);
//auto rand = std::bind(distribution, gen);
//
//// Generate vector with random values
//std::generate(m_randomFloat.begin(), m_randomFloat.end(), rand);
