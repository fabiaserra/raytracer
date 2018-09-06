#include "Hitables/Rectangle.h"

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(float x0, float x1, float y0, float y1, float k, std::shared_ptr<Material> material)
    : m_x0(x0), m_x1(x1), m_y0(y0), m_y1(y1), m_k(k), m_material(material)
{
}

bool Rectangle::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const
{
    float t = (m_k - ray.origin().z()) / ray.direction().z();
    if (t < tMin || t > tMax)
    {
        return false;
    }
    float x = ray.origin().x() + t * ray.direction().x();
    float y = ray.origin().y() + t * ray.direction().y();
    if (x < m_x0 || x > m_x1 || y < m_y0 || y > m_y1)
    {
        return false;
    }
    record.u = (x - m_x0) / (m_x1 - m_x0);
    record.v = (y - m_y0) / (m_y1 - m_y0);
    record.t = t;
    record.material = m_material;
    record.p = ray.pointAtParameter(t);
    record.normal = Vec3(0.f, 0.f, 1.f);

    return true;
}

bool Rectangle::boundingBox(float time0, float time1, AABB &box) const
{
    box = AABB(Vec3(m_x0, m_y0, m_k - 0.0001f), Vec3(m_x1, m_y1, m_k + 0.0001f));
    return true;
}
