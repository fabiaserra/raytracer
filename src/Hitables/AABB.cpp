#include "Hitables/AABB.h"

AABB::AABB()
{
}

AABB::AABB(const Vec3& min, const Vec3& max)
    : m_bounds{min, max}
{
}

bool AABB::hit(const Ray& ray, float tMin, float tMax) const
{
    float txMin, txMax, tyMin, tyMax, tzMin, tzMax;

    txMin = (m_bounds[ray.m_sign[0]].x() - ray.origin().x()) * ray.invDirection().x();
    txMax = (m_bounds[1 - ray.m_sign[0]].x() - ray.origin().x()) * ray.invDirection().x();

    tyMin = (m_bounds[ray.m_sign[1]].y() - ray.origin().y()) * ray.invDirection().y();
    tyMax = (m_bounds[1 - ray.m_sign[1]].y() - ray.origin().y()) * ray.invDirection().y();

    if ((txMin > tyMax) || (tyMin > txMax))
    {
        return false;
    }
    if (tyMin > txMin)
    {
        txMin = tyMin;
    }
    if (tyMax < txMax)
    {
        txMax = tyMax;
    }

    tzMin = (m_bounds[ray.m_sign[2]].z() - ray.origin().z()) * ray.invDirection().z();
    tzMax = (m_bounds[1 - ray.m_sign[2]].z() - ray.origin().z()) * ray.invDirection().z();

    if ((txMin > tzMax) || (tzMin > txMax))
    {
        return false;
    }
    if (tzMin > txMin)
    {
        txMin = tzMin;
    }
    if (tzMax < txMax)
    {
        txMax = tzMax;
    }

    return ((txMin < tMax) && (txMax > tMin));
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
