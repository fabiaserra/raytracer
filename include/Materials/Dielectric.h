#pragma once
#include "Materials/Material.h"

class Dielectric : public Material
{
public:
	Dielectric(float refractive_index);

	// Schlick's approximation, named after Christophe Schlick, is a formula for approximating 
	// contribution of the Fresnel factor in the specular reflection of light from a non-conducting
	// interface (surface) between two media.
	// https://en.wikipedia.org/wiki/Schlick%27s_approximation
	float getSchlickCoeffiecientApproximation(float cosine); 

	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered);

private:
	float m_refractive_index;

};
