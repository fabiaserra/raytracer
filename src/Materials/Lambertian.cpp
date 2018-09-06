#include "Materials/Lambertian.h"

Lambertian::Lambertian(std::shared_ptr<Texture> albedo) : m_albedo(albedo)
{
}

bool Lambertian::scatter(const Ray & rayIn, const HitRecord & record, Vec3 & attenuation, Ray & scattered)
{
	// Random point on unit sphere that is tangent to the hit point
	// https://github.com/aras-p/ToyPathTracer/issues/2
    Vec3 target = record.p + record.normal + m_randomGenerator.randomUnitVector();
    scattered = Ray(record.p, target - record.p, rayIn.time());
    attenuation = m_albedo->value(0.f, 0.f, record.p);
	return true;
}
