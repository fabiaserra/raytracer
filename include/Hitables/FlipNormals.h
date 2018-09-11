#pragma once

#include "Hitables/Hitable.h"

class FlipNormals : public Hitable
{
public:
    FlipNormals();
    FlipNormals(std::shared_ptr<Hitable> pointer);

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    virtual bool boundingBox(float time0, float time1, AABB& box) const;

private:
    std::shared_ptr<Hitable> m_pointer;
};

