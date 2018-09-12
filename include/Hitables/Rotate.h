#pragma once

#include "Hitables/Hitable.h"

class RotateY : public Hitable
{
public:
    RotateY(std::shared_ptr<Hitable> pointer, float angle);

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    virtual bool boundingBox(float time0, float time1, AABB& box) const;

private:
    std::shared_ptr<Hitable> m_pointer;
    float m_sinAngle;
    float m_cosAngle;
    bool m_hasBox;
    AABB m_box;
};
