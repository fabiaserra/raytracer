#include "Core/Ray.h"

Ray::Ray(const Vec3 & origin, const Vec3 & direction, float time) 
	: m_origin(origin), m_direction(direction), m_time(time)
{
}
