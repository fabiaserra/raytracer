#pragma once
#include "Hitables/Hitable.h"
#include "Utils/RandomGenerator.h"

class BVHNode : public Hitable
{
public:
    BVHNode(std::vector<std::shared_ptr<Hitable>> hitables, float time0, float time1);

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    virtual bool boundingBox(float time0, float time1, AABB& box) const;

protected:
    AABB m_box;
    std::shared_ptr<Hitable> m_left;
    std::shared_ptr<Hitable> m_right;
    RandomGenerator m_randomGenerator;
};
