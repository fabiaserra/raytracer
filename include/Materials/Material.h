#pragma once
#include "Hitables/Hitable.h"
#include "Utils/RandomGenerator.h"

class Material
{
public:
    Material();
    virtual ~Material() = 0;
	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) = 0;
    virtual Vec3 emitted(float u, float v, const Vec3& p) const;

	Vec3 reflect(const Vec3& vectorIn, const Vec3& normal);
	bool refract(const Vec3& vectorIn, const Vec3& normal, float n1_over_n2, Vec3& refracted);

protected:
	RandomGenerator m_randomGenerator;
};

inline Vec3 Material::reflect(const Vec3& vectorIn, const Vec3& normal)
{
	return vectorIn - 2 * dot(vectorIn, normal) * normal;
}
