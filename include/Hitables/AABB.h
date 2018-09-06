#pragma once
#include "Core/Ray.h"
#include "Core/Vec3.h"

inline float ffmin(float a, float b)
{
	return a < b ? a : b;
}

inline float ffmax(float a, float b) 
{
	return a > b ? a : b;
}

class AABB
{
public:
	AABB();
	AABB(const Vec3& min, const Vec3& max);

	Vec3 min() const;
	Vec3 max() const;

	bool hit(const Ray& ray, float tMin, float tMax) const;

	static AABB surroundingBox(AABB& box0, AABB& box1);

private:
	Vec3 m_min;
	Vec3 m_max;
};

inline Vec3 AABB::min() const
{
	return m_min;
}

inline Vec3 AABB::max() const
{
	return m_max;
}
