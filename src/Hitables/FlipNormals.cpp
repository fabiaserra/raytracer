#include "Hitables/FlipNormals.h"

FlipNormals::FlipNormals()
{
}

FlipNormals::FlipNormals(std::shared_ptr<Hitable> pointer)
    : m_pointer(pointer)
{
}

bool FlipNormals::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const
{
    if (m_pointer->hit(ray, tMin, tMax, record))
    {
        record.normal = -record.normal;
        return true;
    }
    else
    {
        return false;
    }
}

bool FlipNormals::boundingBox(float time0, float time1, AABB &box) const
{
    return m_pointer->boundingBox(time0, time1, box);
}
