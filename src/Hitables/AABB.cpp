#include "Hitables/AABB.h"

AABB::AABB()
{
}

AABB::AABB(const Vec3& min, const Vec3& max) : m_min(min), m_max(max)
{
}

bool AABB::hit(const Ray & ray, float tMin, float tMax) const
{
	for (int a = 0; a < 3; ++a)
	{
		float directionInverse = 1.f / ray.direction()[a];
		float t0 = (min()[a] - ray.origin()[a]) * directionInverse;
		float t1 = (max()[a] - ray.origin()[a]) * directionInverse;

		if (directionInverse < 0.f)
		{
			std::swap(t0, t1);
		}

		tMin = t0 > tMin ? t0 : tMin;
		tMax = t1 < tMax ? t1 : tMax;

		if (tMax <= tMin)
		{
			return false;
		}
	}

	return true;
}

AABB AABB::surroundingBox(AABB& box0, AABB& box1)
{
	Vec3 min(
		ffmin(box0.min().x(), box1.min().x()),
		ffmin(box0.min().y(), box1.min().y()),
		ffmin(box0.min().z(), box1.min().z()));

	Vec3 max(
		ffmax(box0.max().x(), box1.max().x()),
		ffmax(box0.max().y(), box1.max().y()),
		ffmax(box0.max().z(), box1.max().z()));

	return AABB(min, max);
}
