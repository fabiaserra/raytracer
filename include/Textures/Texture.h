#pragma once
#include "Core/Vec3.h"

class Texture
{
public:
	virtual Vec3 value(float u, float v, const Vec3& p) const = 0;
};
