#include "Materials/Metal.h"

Metal::Metal(const Vec3 & albedo, float fuzz)
    : m_albedo(albedo), m_fuzz(fuzz)
{
    if (fuzz > 1.f)
    {
        m_fuzz = 1.f;
    }
}

bool Metal::scatter(const Ray & rayIn, const HitRecord & record, Vec3 & attenuation, Ray & scattered)
{
    Vec3 reflected = reflect(rayIn.direction(), record.normal);
    scattered = Ray(record.p, reflected + m_fuzz * m_randomGenerator.randomInUnitSphere(), rayIn.time());
	attenuation = m_albedo;
    return (dot(scattered.direction(), record.normal) > 0.f);
}
