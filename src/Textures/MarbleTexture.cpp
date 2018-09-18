#include "Textures/MarbleTexture.h"

MarbleTexture::MarbleTexture(float scale) : m_scale(scale)
{
}

MarbleTexture::~MarbleTexture()
{
}

Vec3 MarbleTexture::value(float u, float v, const Vec3 & p) const
{
    return Vec3(1.f, 1.f, 1.f) * 0.5f *
            (1.f + sinf(m_scale * p.z() + 5.f * m_perlinNoise.octaveNoise(m_scale * p, 6)));
}
