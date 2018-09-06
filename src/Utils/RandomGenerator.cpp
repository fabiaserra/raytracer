#include "Utils/RandomGenerator.h"

RandomGenerator::RandomGenerator(unsigned int seed)
{
	m_engine = pcg32(seed); // seed the engine with 1 by default
	m_floatDistribution = std::uniform_real_distribution<float>{ 0.f, 1.f };
}

Vec3 RandomGenerator::randomInUnitSphere()
{
	Vec3 p;
	do
	{
		p = 2.f * Vec3(nextFloat(), nextFloat(), nextFloat()) - Vec3(1.f, 1.f, 1.f);
	} while (p.squared_length() >= 1.f);
	return p;
}

Vec3 RandomGenerator::randomInUnitDisk()
{
	Vec3 p;
	do
	{
		p = 2.f * Vec3(nextFloat(), nextFloat(), 0.f) - Vec3(1.f, 1.f, 0.f);
	} while (p.squared_length() >= 1.f);
	return p;
}

Vec3 RandomGenerator::randomUnitVector()
{
	float z = nextFloat() * 2.f - 1.f;
	float a = nextFloat() * 2.f * static_cast<float>(M_PI);
	float r = sqrtf(1.f - z * z);
	float x = r * cosf(a);
	float y = r * sinf(a);
	return Vec3(x, y, z);
}

