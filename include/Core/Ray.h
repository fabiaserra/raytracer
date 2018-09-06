#pragma once
#include "Core/Vec3.h"

class Ray
{
public:
	Ray() {}
	Ray(const Vec3& origin, const Vec3& direction, float time = 0.f);

	Vec3 origin() const;
	Vec3 direction() const;
	Vec3 pointAtParameter(float t) const;
	float time() const;

private:
	Vec3 m_origin;
	Vec3 m_direction;
	float m_time;
};

inline Vec3 Ray::origin() const
{
	return m_origin;
}

inline Vec3 Ray::direction() const
{
	return m_direction;
}

inline Vec3 Ray::pointAtParameter(float t) const
{ 
	return m_origin + t * m_direction;
}

inline float Ray::time() const
{
	return m_time;
}
