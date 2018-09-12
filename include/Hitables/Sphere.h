#pragma once

#include "Hitables/Hitable.h"

class Sphere : public Hitable
{
public:
    Sphere(Vec3 center, float radius, std::shared_ptr<Material> material);

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    virtual bool boundingBox(float time0, float time1, AABB& box) const;

private:
    Vec3 m_center;
    float m_radius;
    std::shared_ptr<Material> m_material;
};
