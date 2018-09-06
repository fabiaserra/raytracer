#include "Hitables/Sphere.h"

void getSphereUV(const Vec3 &p, float& u, float& v)
{
    float phi = atan2f(p.z(), p.x());
    float theta = asinf(p.y());
    u = 1.f - (phi + PI) / (2.f * PI);
    v = (theta + PI / 2.f) / PI;
}

Sphere::Sphere(Vec3 center, float radius, std::shared_ptr<Material> material) :
	m_center(center), m_radius(radius), m_material(std::move(material))
{
}

bool Sphere::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	Vec3 originToCenter = ray.origin() - m_center;
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
            getSphereUV((record.p - m_center) / m_radius, record.u, record.v);
			record.normal = (record.p - m_center) / m_radius;
			record.material = m_material;
			return true;
		}
		temp = (-b + root) / (2.f*a);
		if (temp < tMax && temp > tMin)
		{
			record.t = temp;
			record.p = ray.pointAtParameter(temp);
            getSphereUV((record.p - m_center) / m_radius, record.u, record.v);
			record.normal = (record.p - m_center) / m_radius;
			record.material = m_material;
			return true;
		}
	}

	return false;
}

bool Sphere::boundingBox(float time0, float time1, AABB & box) const
{
	box = AABB(m_center - Vec3(m_radius, m_radius, m_radius), m_center + Vec3(m_radius, m_radius, m_radius));
	return true;
}
