#include "Textures/NoiseTexture.h"

NoiseTexture::NoiseTexture(float scale) : m_scale(scale)
{
}

NoiseTexture::~NoiseTexture()
{
}

Vec3 NoiseTexture::value(float u, float v, const Vec3 & p) const
{
    return Vec3(1.f, 1.f, 1.f) * m_perlinNoise.octaveNoise(m_scale * p, 4);
}
