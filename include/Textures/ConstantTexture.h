#pragma once
#include "Textures/Texture.h"

class ConstantTexture : public Texture
{
public:
	ConstantTexture();
	ConstantTexture(Vec3 color);
    virtual ~ConstantTexture();

	virtual Vec3 value(float u, float v, const Vec3& p) const;

private:
	Vec3 m_color;
};

