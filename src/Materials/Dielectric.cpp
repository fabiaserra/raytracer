#include "Materials/Dielectric.h"

Dielectric::Dielectric(float refractive_index) : m_refractive_index(refractive_index)
{
}

float Dielectric::getSchlickCoeffiecientApproximation(float cosine)
{
    float r0 = (1.f - m_refractive_index) / (1.f + m_refractive_index);
    r0 = r0 * r0;
    float one_minus_cosine = 1.f - cosine;
    return r0 + (1.f - r0) * one_minus_cosine * one_minus_cosine * one_minus_cosine * one_minus_cosine * one_minus_cosine;
}

bool Dielectric::scatter(const Ray & rayIn, const HitRecord & record, Vec3 & attenuation, Ray & scattered)
{
    Vec3 outward_normal;
    Vec3 reflected = reflect(rayIn.direction(), record.normal);
    float n1_over_n2;
    attenuation = Vec3(1.f, 1.f, 1.f);
    float percentage_of_reflection;
    float cosine;
    float ray_dot_normal = dot(rayIn.direction(), record.normal);

    // we are inside the surface, we need to revert the normal direction
    if (ray_dot_normal > 0.f)
    {
        outward_normal = -record.normal;
        n1_over_n2 = m_refractive_index;
        cosine = ray_dot_normal / rayIn.direction().length();
        cosine = sqrtf(1.f - m_refractive_index * m_refractive_index * (1.f - cosine * cosine));
    }
    // we are outside the surface, we revert the index of refraction and we want the cos(theta) to be positive
    else
    {
        outward_normal = record.normal;
        n1_over_n2 = 1.f / m_refractive_index;
        cosine = -ray_dot_normal / rayIn.direction().length();
    }

    Vec3 refracted;
    if (refract(rayIn.direction(), outward_normal, n1_over_n2, refracted))
    {
        percentage_of_reflection = getSchlickCoeffiecientApproximation(cosine);
    }
    else
    {
        percentage_of_reflection = 1.f;
    }

    if (m_randomGenerator.nextFloat() < percentage_of_reflection)
    {
        scattered = Ray(record.p, reflected, rayIn.time());
    }
    else
    {
        scattered = Ray(record.p, refracted, rayIn.time());
    }
    return true;
}
