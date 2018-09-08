#include "Textures/CheckerTexture.h"

CheckerTexture::CheckerTexture()
{
}

CheckerTexture::CheckerTexture(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1)
	: m_odd(t0), m_even(t1)
{
}

CheckerTexture::~CheckerTexture()
{
}

Vec3 CheckerTexture::value(float u, float v, const Vec3& p) const
{
	float sines = sinf(10.f * p.x()) * sinf(10.f * p.y()) * sinf(10.f * p.z());
	if (sines < 0.f)
	{
		return m_odd->value(u, v, p);
	}
	else
	{
		return m_even->value(u, v, p);
	}
}
