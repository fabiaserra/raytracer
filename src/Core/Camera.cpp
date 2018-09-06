#include "Core/Camera.h"

Camera::Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 viewUp, float verticalFov,
	float aspectRatio, float aperture, float focusDistance, float shutterOpen,
	float shutterClose)
{
	m_shutterOpen = shutterOpen;
	m_shutterClose = shutterClose;
	m_lensRadius = aperture / 2.f;
	float theta = verticalFov * static_cast<float>(M_PI) / 180.f;
	float halfHeight = tan(theta / 2.f);
	float halfWidth = halfHeight * aspectRatio;
	m_origin = lookFrom;
	m_w = unitVector(lookFrom - lookAt);
	m_u = unitVector(cross(viewUp, m_w));
	m_v = cross(m_w, m_u);
	m_lowerLeftCorner = m_origin - (halfWidth * focusDistance * m_u) - (halfHeight * focusDistance * m_v) - (focusDistance * m_w);
	m_horizontal = 2.f * halfWidth * focusDistance * m_u;
	m_vertical = 2.f * halfHeight * focusDistance * m_v;
	m_randomGenerator = RandomGenerator();
}

Ray Camera::getRay(float s, float t)
{
	Vec3 randomDisk = m_lensRadius * m_randomGenerator.randomInUnitDisk();
	Vec3 offset = m_u * randomDisk.x() + m_v * randomDisk.y();
	float time = m_shutterOpen + m_randomGenerator.nextFloat() * (m_shutterClose - m_shutterOpen);
	return Ray(m_origin + offset, m_lowerLeftCorner + (s * m_horizontal) + (t * m_vertical) - m_origin - offset, time);
}

