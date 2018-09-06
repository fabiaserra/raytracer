#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#include "Core/Ray.h"
#include "Utils/RandomGenerator.h"

class Camera
{
public:
	/**
		@param lookFrom Position in world space of the camera
		@param lookAt Position where the camera faces to
		@param viewUp Up vector of the camera
		@param verticalFov Top to bottom in degrees
		@param aspectRatio Ratio width/height of the camera
		@param aperture Opening of lens to control depth of field
		@param focusDistance Distance where objects will be in focus
		@param shutterOpen Time when shutter opens
		@param shutterClose Time when shutter closes
	*/
	Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 viewUp, float verticalFov,
		float aspectRatio, float aperture, float focusDistance,
		float shutterOpen, float shutterClose);

	Ray getRay(float s, float t);

private:
	Vec3 m_origin;
	Vec3 m_lowerLeftCorner;
	Vec3 m_horizontal;
	Vec3 m_vertical;
	
	Vec3 m_u;
	Vec3 m_v;
	Vec3 m_w;
	
	float m_lensRadius;
	
	float m_shutterOpen;
	float m_shutterClose;

	RandomGenerator m_randomGenerator;
};
