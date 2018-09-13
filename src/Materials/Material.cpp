#include "Materials/Material.h"

Material::Material()
{
}

Material::~Material()
{
}

Vec3 Material::emitted(float u, float v, const Vec3 &p) const
{
    return Vec3(0.f, 0.f, 0.f);
}

bool Material::refract(const Vec3 & vectorIn, const Vec3 & normal, float n1_over_n2, Vec3 & refracted)
{
	Vec3 vectorDirection = unitVector(vectorIn);
	float cosIncidance = dot(vectorDirection, normal);
	float discriminant = 1.f - (n1_over_n2 * n1_over_n2) * (1.f - (cosIncidance * cosIncidance));
	if (discriminant > 0.f)
	{
		refracted = n1_over_n2 * vectorDirection - normal * (n1_over_n2 * cosIncidance + sqrt(discriminant));
		return true;
	}
	else
	{
		return false;
	}
}
