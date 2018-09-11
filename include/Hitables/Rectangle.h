#pragma once

#include "Hitables/Hitable.h"

/******************************************
 * XY Rectangle
 *****************************************/
class XYRectangle : public Hitable
{
public:
    XYRectangle();
    XYRectangle(float x0, float x1, float y0, float y1, float k, std::shared_ptr<Material> material);

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    virtual bool boundingBox(float time0, float time1, AABB& box) const;

private:
    float m_x0, m_x1, m_y0, m_y1, m_k;
    std::shared_ptr<Material> m_material;
};

/******************************************
 * XZ Rectangle
 *****************************************/
class XZRectangle : public Hitable
{
public:
    XZRectangle();
    XZRectangle(float x0, float x1, float z0, float z1, float k, std::shared_ptr<Material> material);

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    virtual bool boundingBox(float time0, float time1, AABB& box) const;

private:
    float m_x0, m_x1, m_z0, m_z1, m_k;
    std::shared_ptr<Material> m_material;
};

/******************************************
 * YZ Rectangle
 *****************************************/
class YZRectangle : public Hitable
{
public:
    YZRectangle();
    YZRectangle(float y0, float y1, float z0, float z1, float k, std::shared_ptr<Material> material);

    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    virtual bool boundingBox(float time0, float time1, AABB& box) const;

private:
    float m_y0, m_y1, m_z0, m_z1, m_k;
    std::shared_ptr<Material> m_material;
};

