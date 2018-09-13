#include "Hitables/ConstantMedium.h"

ConstantMedium::ConstantMedium(std::shared_ptr<Hitable> boundary,
                               float density,
                               std::shared_ptr<Texture> texture)
    : m_boundary(boundary)
    , m_density(density)
    , m_phaseFunction(std::make_shared<Isotropic>(texture))
{
}

bool ConstantMedium::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
    HitRecord record1, record2;
    if (m_boundary->hit(ray, -FLT_MAX, FLT_MAX, record1))
    {
        if (m_boundary->hit(ray, record1.t + 0.0001f, FLT_MAX, record2))
        {
            if (record1.t < tMin)
            {
                record1.t = tMin;
            }
            if (record2.t > tMax)
            {
                record2.t = tMax;
            }
            if (record1.t >= record2.t)
            {
                return false;
            }
            if (record1.t < 0.f)
            {
                record1.t = 0.f;
            }
            float distanceInsideBoundary = (record2.t - record1.t) * ray.direction().length();
            float hitDistance = - (1.f / m_density) * log(RandomGenerator::randFloat());
            if (hitDistance < distanceInsideBoundary)
            {
                record.t = record1.t + hitDistance / ray.direction().length();
                record.p = ray.pointAtParameter(record.t);
                record.normal = Vec3(1.f, 0.f, 0.f); // arbitrary
                record.material = m_phaseFunction;
                return true;
            }
        }
    }
    return false;
}

bool ConstantMedium::boundingBox(float time0, float time1, AABB & box) const
{
    return m_boundary->boundingBox(time0, time1, box);
}
