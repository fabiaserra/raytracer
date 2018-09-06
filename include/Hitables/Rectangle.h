#pragma once

#include "Hitables/Hitable.h"

class Rectangle : public Hitable
{
public:
    Rectangle();
    Rectangle(float x0, float x1, float y0, float y1, float k, std::shared_ptr<Material> material);

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    virtual bool boundingBox(float time0, float time1, AABB& box) const;

private:
    float m_x0, m_x1, m_y0, m_y1, m_k;
    std::shared_ptr<Material> m_material;
};

