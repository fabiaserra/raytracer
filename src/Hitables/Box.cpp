#include "Hitables/Box.h"

Box::Box(Vec3& pMin, Vec3& pMax, std::shared_ptr<Material> material)
    : m_pMin(pMin), m_pMax(pMax)
{
    std::vector<std::shared_ptr<Hitable>> rectangles(6);
    rectangles[0] = std::make_shared<XYRectangle>(pMin.x(), pMax.x(), pMin.y(), pMax.y(), pMax.z(), material);
    rectangles[1] = std::make_shared<FlipNormals>(std::make_shared<XYRectangle>(pMin.x(), pMax.x(), pMin.y(), pMax.y(), pMin.z(), material));
    rectangles[2] = std::make_shared<XZRectangle>(pMin.x(), pMax.x(), pMin.z(), pMax.z(), pMax.y(), material);
    rectangles[3] = std::make_shared<FlipNormals>(std::make_shared<XZRectangle>(pMin.x(), pMax.x(), pMin.z(), pMax.z(), pMin.y(), material));
    rectangles[4] = std::make_shared<YZRectangle>(pMin.y(), pMax.y(), pMin.z(), pMax.z(), pMax.x(), material);
    rectangles[5] = std::make_shared<FlipNormals>(std::make_shared<YZRectangle>(pMin.y(), pMax.y(), pMin.z(), pMax.z(), pMin.x(), material));

    m_rectangles = std::make_shared<HitableList>(rectangles);
}

bool Box::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
    return m_rectangles->hit(ray, tMin, tMax, record);
}

bool Box::boundingBox(float time0, float time1, AABB & box) const
{
    box = AABB(m_pMin, m_pMax);
	return true;
}
