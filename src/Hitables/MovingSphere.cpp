#include "Hitables/MovingSphere.h"

MovingSphere::MovingSphere(Vec3 centerStart, Vec3 centerEnd, float timeStart,
	float timeEnd, float radius, std::shared_ptr<Material> material)
	: m_centerStart(centerStart)
	, m_centerEnd(centerEnd)
	, m_timeStart(timeStart)
	, m_timeEnd(timeEnd)
	, m_radius(radius)
	, m_material(std::move(material))
{
}

Vec3 MovingSphere::center(float time) const
{
	return m_centerStart + ((time - m_timeStart) / (m_timeEnd - m_timeStart)) * (m_centerStart - m_centerEnd);
}

bool MovingSphere::hit(const Ray & ray, float tMin, float tMax, HitRecord & record) const
{
	Vec3 originToCenter = ray.origin() - center(ray.time());
	float a = dot(ray.direction(), ray.direction());
	float b = 2.f * dot(originToCenter, ray.direction());
	float c = dot(originToCenter, originToCenter) - m_radius * m_radius;
	float discriminant = b * b - 4 * a*c;
	if (discriminant > 0)
	{
		float root = sqrt(discriminant);
		float temp = (-b - root) / (2.f*a);
		if (temp < tMax && temp > tMin)
		{
			record.t = temp;
			record.p = ray.pointAtParameter(temp);
			record.normal = (record.p - center(ray.time())) / m_radius;
			record.material = m_material;
			return true;
		}
		temp = (-b + root) / (2.f*a);
		if (temp < tMax && temp > tMin)
		{
			record.t = temp;
			record.p = ray.pointAtParameter(temp);
			record.normal = (record.p - center(ray.time())) / m_radius;
			record.material = m_material;
			return true;
		}
	}

	return false;
}

inline bool MovingSphere::boundingBox(float time0, float time1, AABB& box) const
{
	AABB box_start = AABB(m_centerStart - Vec3(m_radius, m_radius, m_radius), m_centerStart + Vec3(m_radius, m_radius, m_radius));
	AABB box_end = AABB(m_centerEnd - Vec3(m_radius, m_radius, m_radius), m_centerEnd + Vec3(m_radius, m_radius, m_radius));
	box = AABB::surroundingBox(box_start, box_end);
	return true;
}
