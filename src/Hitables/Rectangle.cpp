#include "Hitables/Rectangle.h"

/******************************************
 * XY Rectangle
 *****************************************/
XYRectangle::XYRectangle()
{
}

XYRectangle::XYRectangle(float x0, float x1, float y0, float y1, float k, std::shared_ptr<Material> material)
    : m_x0(x0), m_x1(x1), m_y0(y0), m_y1(y1), m_k(k), m_material(material)
{
}

bool XYRectangle::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const
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

bool XYRectangle::boundingBox(float time0, float time1, AABB &box) const
{
    box = AABB(Vec3(m_x0, m_y0, m_k - 0.0001f), Vec3(m_x1, m_y1, m_k + 0.0001f));
    return true;
}

/******************************************
 * XZ Rectangle
 *****************************************/
XZRectangle::XZRectangle()
{
}

XZRectangle::XZRectangle(float x0, float x1, float z0, float z1, float k, std::shared_ptr<Material> material)
    : m_x0(x0), m_x1(x1), m_z0(z0), m_z1(z1), m_k(k), m_material(material)
{
}

bool XZRectangle::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const
{
    float t = (m_k - ray.origin().z()) / ray.direction().z();
    if (t < tMin || t > tMax)
    {
        return false;
    }
    float x = ray.origin().x() + t * ray.direction().x();
    float z = ray.origin().z() + t * ray.direction().z();
    if (x < m_x0 || x > m_x1 || z < m_z0 || z > m_z1)
    {
        return false;
    }
    record.u = (x - m_x0) / (m_x1 - m_x0);
    record.v = (z - m_z0) / (m_z1 - m_z0);
    record.t = t;
    record.material = m_material;
    record.p = ray.pointAtParameter(t);
    record.normal = Vec3(0.f, 0.f, 1.f);

    return true;
}

bool XZRectangle::boundingBox(float time0, float time1, AABB &box) const
{
    box = AABB(Vec3(m_x0, m_z0, m_k - 0.0001f), Vec3(m_x1, m_z1, m_k + 0.0001f));
    return true;
}

/******************************************
 * YZ Rectangle
 *****************************************/
YZRectangle::YZRectangle()
{
}

YZRectangle::YZRectangle(float y0, float y1, float z0, float z1, float k, std::shared_ptr<Material> material)
    : m_y0(y0), m_y1(y1), m_z0(z0), m_z1(z1), m_k(k), m_material(material)
{
}

bool YZRectangle::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const
{
    float t = (m_k - ray.origin().z()) / ray.direction().z();
    if (t < tMin || t > tMax)
    {
        return false;
    }
    float y = ray.origin().y() + t * ray.direction().y();
    float z = ray.origin().z() + t * ray.direction().z();
    if (y < m_y0 || y > m_y1 || z < m_z0 || z > m_z1)
    {
        return false;
    }
    record.u = (y - m_y0) / (m_y1 - m_y0);
    record.v = (z - m_z0) / (m_z1 - m_z0);
    record.t = t;
    record.material = m_material;
    record.p = ray.pointAtParameter(t);
    record.normal = Vec3(0.f, 0.f, 1.f);

    return true;
}

bool YZRectangle::boundingBox(float time0, float time1, AABB &box) const
{
    box = AABB(Vec3(m_y0, m_z0, m_k - 0.0001f), Vec3(m_y1, m_z1, m_k + 0.0001f));
    return true;
}
