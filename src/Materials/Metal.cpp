#include "Materials/Metal.h"

Metal::Metal(const Vec3 & albedo, float fuzz) : m_albedo(albedo)
{
	if (fuzz < 1)
	{
		m_fuzz = fuzz;
	}
	else
	{
		m_fuzz = 1;
	}
}

bool Metal::scatter(const Ray & rayIn, const HitRecord & record, Vec3 & attenuation, Ray & scattered)
{
    Vec3 reflected = reflect(rayIn.direction(), record.normal);
    scattered = Ray(record.p, reflected + m_fuzz * m_randomGenerator.randomInUnitSphere(), rayIn.time());
	attenuation = m_albedo;
    return (dot(scattered.direction(), record.normal) > 0.f);
}
