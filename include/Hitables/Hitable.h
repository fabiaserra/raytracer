#pragma once
#include "Core/Ray.h"
#include "Hitables/AABB.h"

class Material;

struct HitRecord
{
	float t;
    float u;
    float v;
	Vec3 p;
	Vec3 normal;
	std::shared_ptr<Material> material;
};

class Hitable
{
public:
    virtual ~Hitable() {}

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const = 0;
	virtual bool boundingBox(float time0, float time1, AABB& box) const = 0;
};
