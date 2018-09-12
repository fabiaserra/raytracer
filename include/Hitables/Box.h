#pragma once

#include "Hitables/Hitable.h"
#include "Hitables/HitableList.h"
#include "Hitables/Rectangle.h"
#include "Hitables/FlipNormals.h"

class Box : public Hitable
{
public:
    Box(Vec3& pmin, Vec3& pmax, std::shared_ptr<Material> material);

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    virtual bool boundingBox(float time0, float time1, AABB& box) const;

private:
    Vec3 m_pMin;
    Vec3 m_pMax;
    std::shared_ptr<HitableList> m_rectangles;
};
