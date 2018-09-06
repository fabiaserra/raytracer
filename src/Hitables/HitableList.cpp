#include "Hitables/HitableList.h"

HitableList::HitableList(std::vector<std::shared_ptr<Hitable>>& hitables)
	: m_hitables(hitables)
{
}

bool HitableList::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	bool hitAnything = false;
	float closestSoFar = tMax;
	for (auto& hitable : m_hitables)
	{
        if (hitable->hit(ray, tMin, closestSoFar, record))
		{
			hitAnything = true;
            closestSoFar = record.t;
		}
	}
	return hitAnything;
}

bool HitableList::boundingBox(float time0, float time1, AABB & box) const
{
	if (m_hitables.size() < 1) return false;
	
	AABB tempBox;
	bool first_true = m_hitables[0]->boundingBox(time0, time1, tempBox);
	if (first_true)
	{
		box = tempBox;
	}
	else
	{
		return false;
	}

	for (auto& hitable : m_hitables)
	{
		if (hitable->boundingBox(time0, time1, tempBox))
		{
			box = AABB::surroundingBox(box, tempBox);
		}
		else
		{
			return false;
		}
	}

	return true;
}
