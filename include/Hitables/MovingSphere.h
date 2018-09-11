#pragma once
#include "Hitables/Hitable.h"

class MovingSphere : public Hitable
{
public:
    MovingSphere(Vec3 centerStart, Vec3 centerEnd, float timeStart,
                 float timeEnd, float radius, std::shared_ptr<Material> material);

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    virtual bool boundingBox(float time0, float time1, AABB& box) const;

    Vec3 center(float time) const;

private:
    Vec3 m_centerStart;
    Vec3 m_centerEnd;

    float m_radius;

    float m_timeStart;
    float m_timeEnd;

    std::shared_ptr<Material> m_material;
};

