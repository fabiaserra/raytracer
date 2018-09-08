#include "Textures/ConstantTexture.h"


ConstantTexture::ConstantTexture()
{
}

ConstantTexture::ConstantTexture(Vec3 color) : m_color(color)
{
}

ConstantTexture::~ConstantTexture()
{
}

Vec3 ConstantTexture::value(float u, float v, const Vec3& p) const
{
	return m_color;
}


