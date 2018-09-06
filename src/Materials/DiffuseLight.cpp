#include "Materials/DiffuseLight.h"

DiffuseLight::DiffuseLight(std::shared_ptr<Texture> emit) : m_emit(emit)
{
}

bool DiffuseLight::scatter(const Ray &rayIn, const HitRecord &rec, Vec3 &attenuation, Ray &scattered)
{
    return false;
}

Vec3 DiffuseLight::emitted(float u, float v, const Vec3 &p) const
{
    return m_emit->value(u, v, p);
}

