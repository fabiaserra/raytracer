#pragma once
#include "Core/Ray.h"
#include "Core/Vec3.h"

inline float ffmin(float a, float b)
{
    return a < b ? a : b;
}

inline float ffmax(float a, float b) 
{
    return a > b ? a : b;
}

// Modifications inspired by Amy Williams BVH Optimization
// http://people.csail.mit.edu/amy/papers/box-jgt.pdf
class AABB
{
public:
    AABB();
    AABB(const Vec3& min, const Vec3& max);

    Vec3 min() const;
    Vec3 max() const;

    bool hit(const Ray& ray, float tMin, float tMax) const;

    static AABB surroundingBox(AABB& box0, AABB& box1);

private:
    Vec3 m_bounds[2];
};

inline Vec3 AABB::min() const
{
    return m_bounds[0];
}

inline Vec3 AABB::max() const
{
    return m_bounds[1];
}
