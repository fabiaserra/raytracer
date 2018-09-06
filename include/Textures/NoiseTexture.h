#pragma once
#include "Textures/Texture.h"
#include "Utils/PerlinNoise.h"

class NoiseTexture : public Texture
{
public:
	NoiseTexture(float scale = 1.f);
	virtual Vec3 value(float u, float v, const Vec3& p) const;
	
private:
	float m_scale;
	PerlinNoise m_perlinNoise;
};
