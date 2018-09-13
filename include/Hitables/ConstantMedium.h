#pragma once

#include "Hitables/Hitable.h"
#include "Materials/Isotropic.h"
//#include "Utils/RandomGenerator.h"

class ConstantMedium : public Hitable
{
public:
    ConstantMedium(std::shared_ptr<Hitable> boundary, float density, std::shared_ptr<Texture> texture);

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    virtual bool boundingBox(float time0, float time1, AABB& box) const;

private:   
    std::shared_ptr<Hitable> m_boundary;
    float m_density;
    std::shared_ptr<Material> m_phaseFunction;
};
