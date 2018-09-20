#include "Core/Ray.h"

Ray::Ray(const Vec3 & origin, const Vec3 & direction, float time) 
    : m_origin(origin)
    , m_direction(direction)
    , m_invDirection(Vec3(1.f / direction.x(), 1.f / direction.y(), 1.f / direction.z()))
    , m_time(time)
    , m_sign{(1.f / direction.x()) < 0.f, (1.f / direction.y()) < 0.f, (1.f / direction.z()) < 0.f}
{
}
