#pragma once
#include "Materials/Material.h"
#include "Textures/Texture.h"

class Lambertian : public Material
{
public:
	Lambertian(std::shared_ptr<Texture> albedo);

	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered);

private:
	std::shared_ptr<Texture> m_albedo;
};
