#pragma once
#include "Hitables/Hitable.h"
#include <vector>

class HitableList : public Hitable
{
public:
    HitableList(std::vector<std::shared_ptr<Hitable>>& hitables);

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    virtual bool boundingBox(float time0, float time1, AABB& box) const;

private:
    std::vector<std::shared_ptr<Hitable>> m_hitables;
};
