#include "Materials/Isotropic.h"

Isotropic::Isotropic(std::shared_ptr<Texture> albedo) : m_albedo(albedo)
{
}

bool Isotropic::scatter(const Ray & rayIn, const HitRecord & record,
                        Vec3 & attenuation, Ray & scattered)
{
    scattered = Ray(record.p, m_randomGenerator.randomInUnitSphere());
    attenuation = m_albedo->value(record.u, record.v, record.p);
    return true;
}
