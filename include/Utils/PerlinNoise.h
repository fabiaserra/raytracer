#pragma once
#include <vector>
#include <random>
#include <numeric>
#include <functional>
#include <algorithm>

#include "Utils/RandomGenerator.h"
#include "Core/Vec3.h"

// Good explanation of perlin noise: http://flafla2.github.io/2014/08/09/perlinnoise.html
// Code very much extracted from here with few modifications
// https://github.com/sol-prog/Perlin_Noise/blob/master/PerlinNoise.cpp

class PerlinNoise
{
public:
	// Initialize with the reference values for the permutation vector
	PerlinNoise();

	// Generate a new permutation vector based on the value of seed
	PerlinNoise(unsigned int seed);

	// Get a noise value
	float noise(const Vec3& p) const;

	// Get a noise value by adding up different octaves of noise
	float octaveNoise(const Vec3& p, int octaves) const;

	float fade(float t) const;
	float lerp(float t, float a, float b) const;
	float grad(int hash, float x, float y, float z) const;
	
private:
	// the permutation vector
	std::vector<int> m_perm;
};
