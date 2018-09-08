#pragma once
#include "Textures/Texture.h"
#include "Utils/PerlinNoise.h"

class MarbleTexture : public Texture
{
public:
    MarbleTexture(float scale = 1.f);
    virtual ~MarbleTexture();

	virtual Vec3 value(float u, float v, const Vec3& p) const;
	
private:
	float m_scale;
	PerlinNoise m_perlinNoise;
};
