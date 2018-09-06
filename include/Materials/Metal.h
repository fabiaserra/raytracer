#pragma once
#include "Materials/Material.h"

class Metal : public Material
{
public:
	Metal(const Vec3& albedo, float fuzz);

	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered);

private:
	Vec3 m_albedo;
	float m_fuzz;
}; 
