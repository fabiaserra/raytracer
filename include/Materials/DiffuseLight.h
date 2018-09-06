#pragma once
#include "Materials/Material.h"
#include "Textures/Texture.h"

class DiffuseLight : public Material
{
public:
    DiffuseLight(std::shared_ptr<Texture> emit);

    virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered);
    virtual Vec3 emitted(float u, float v, const Vec3& p) const;

private:
    std::shared_ptr<Texture> m_emit;
};

