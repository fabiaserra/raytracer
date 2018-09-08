#pragma once
#include "Textures/Texture.h"

class CheckerTexture : public Texture
{
public:
	CheckerTexture();
	CheckerTexture(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1);
    virtual ~CheckerTexture();

	virtual Vec3 value(float u, float v, const Vec3& p) const;

private:
	std::shared_ptr<Texture> m_odd;
	std::shared_ptr<Texture> m_even;
};

