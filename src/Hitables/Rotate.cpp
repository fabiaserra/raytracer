#include "Hitables/Rotate.h"

RotateY::RotateY(std::shared_ptr<Hitable> pointer, float angle)
    : m_pointer(pointer)
{
    float radians = (PI / 180.f) * angle;
    m_sinAngle = sinf(radians);
    m_cosAngle = cosf(radians);
    m_hasBox = m_pointer->boundingBox(0.f, 1.f, m_box);
    Vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    Vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                float x = i * m_box.max().x() + (1 - i) * m_box.min().x();
                float y = j * m_box.max().x() + (1 - j) * m_box.min().y();
                float z = k * m_box.max().x() + (1 - k) * m_box.min().z();
                float newX = m_cosAngle * x + m_sinAngle * z;
                float newZ = -m_sinAngle * x + m_cosAngle * z;
                Vec3 tester(newX, y, newZ);
                for (int c = 0; c < 3; ++c)
                {
                    if (tester[c] > max[c])
                    {
                        max[c] = tester[c];
                    }
                    if (tester[c] < min[c])
                    {
                        min[c] = tester[c];
                    }
                }
            }
        }
    }
    m_box = AABB(min, max);
}

bool RotateY::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
    Vec3 origin = ray.origin();
    Vec3 direction = ray.direction();
    origin[0] = m_cosAngle * ray.origin().x() - m_sinAngle * ray.origin().z();
    origin[2] = m_sinAngle * ray.origin().x() + m_cosAngle * ray.origin().z();
    direction[0] = m_cosAngle * ray.direction().x() - m_sinAngle * ray.direction().z();
    direction[2] = m_sinAngle * ray.direction().x() + m_cosAngle * ray.direction().z();
    Ray rotatedRay(origin, direction, ray.time());
    if (m_pointer->hit(rotatedRay, tMin, tMax, record))
    {
        Vec3 p = record.p;
        p[0] = m_cosAngle * record.p.x() - m_sinAngle * record.p.z();
        p[2] = -m_sinAngle * record.p.x() + m_cosAngle * record.p.z();
        Vec3 normal = record.normal;
        normal[0] = m_cosAngle * record.normal.x() - m_sinAngle * record.normal.z();
        normal[2] = -m_sinAngle * record.normal.x() + m_cosAngle * record.normal.z();
        record.p = p;
        record.normal = normal;
        return true;
    }
    else
    {
        return false;
    }
}

bool RotateY::boundingBox(float time0, float time1, AABB & box) const
{
    box = m_box;
    return m_hasBox;
}
