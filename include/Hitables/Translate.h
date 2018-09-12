#pragma once

#include "Hitables/Hitable.h"

class Translate : public Hitable
{
public:
    Translate(std::shared_ptr<Hitable> pointer, Vec3 offset);

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    virtual bool boundingBox(float time0, float time1, AABB& box) const;

private:
    std::shared_ptr<Hitable> m_pointer;
    Vec3 m_offset;
};
