#include "Hitables/Translate.h"

Translate::Translate(std::shared_ptr<Hitable> pointer, Vec3 offset)
    : m_pointer(pointer), m_offset(offset)
{
}

bool Translate::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
    Ray translatedRay(ray.origin() - m_offset, ray.direction(), ray.time());
    if (m_pointer->hit(translatedRay, tMin, tMax, record))
    {
        record.p += m_offset;
        return true;
    }
    else
    {
        return false;
    }
}

bool Translate::boundingBox(float time0, float time1, AABB & box) const
{
    if (m_pointer->boundingBox(time0, time1, box))
    {
        box = AABB(box.min() + m_offset, box.max() + m_offset);
        return true;
    }
    else
    {
        return false;
    }
}
