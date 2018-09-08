#include <iostream>

#include "Core/Camera.h"

#include "Hitables/Rectangle.h"
#include "Hitables/Sphere.h"
#include "Hitables/MovingSphere.h"
#include "Hitables/HitableList.h"
#include "Hitables/BVHNode.h"

#include "Materials/DiffuseLight.h"
#include "Materials/Dielectric.h"
#include "Materials/Lambertian.h"
#include "Materials/Metal.h"

#include "Textures/ConstantTexture.h"
#include "Textures/CheckerTexture.h"
#include "Textures/MarbleTexture.h"
#include "Textures/NoiseTexture.h"

#include "Utils/RandomGenerator.h"
#include "Utils/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "Utils/stb_image_write.h"

//#define MOVING_SPHERES

Vec3 getColor(const Ray& ray, BVHNode& world, int depth)
{
    HitRecord record;
    if (world.hit(ray, 0.001f, FLT_MAX, record))
	{
		Ray scattered;
		Vec3 attenuation;
        Vec3 emitted = record.material->emitted(record.u, record.v, record.p);
        if (depth < 50 && record.material->scatter(ray, record, attenuation, scattered))
		{
            return emitted + attenuation * getColor(scattered, world, depth + 1);
		}
		else
		{
            return emitted;
		}
	}
	else
	{
        return Vec3(0.f, 0.f, 0.f);
//		Vec3 unit_direction = unitVector(ray.direction());
//		float t = 0.5f * (unit_direction.y() + 1.f);
//		return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
	}
}

BVHNode createSimpleLightScene()
{
    std::vector<std::shared_ptr<Hitable>> hitables(4);

    hitables[0] = std::make_shared<Sphere>(Vec3(0.f, -1000.f, -1.f), 1000.f,
        std::make_shared<Lambertian>(std::make_shared<NoiseTexture>(1.0f)));

    hitables[1] = std::make_shared<Sphere>(Vec3(0.f, 2.f, 0.f), 2.f,
        std::make_shared<Lambertian>(std::make_shared<MarbleTexture>(4.f)));

    hitables[2] = std::make_shared<Sphere>(Vec3(0.f, 8.f, 0.f), 2.f,
        std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(Vec3(4.f, 4.f, 4.f))));

    hitables[3] = std::make_shared<Rectangle>(-2.f, 2.f, 1.f, 4.f, -4.f,
        std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(Vec3(4.f, 4.f, 4.f))));

    return BVHNode(hitables, 0.f, 1.f);
}

BVHNode createSimpleScene()
{
    std::vector<std::shared_ptr<Hitable>> hitables(5);
	hitables[0] = std::make_shared<Sphere>(Vec3(0.f, 0.f, -1.f), 0.5f,
		std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3(0.8f, 0.3f, 0.3f))));

	hitables[1] = std::make_shared<Sphere>(Vec3(0.f, -100.5f, -1.f), 100.f,
		std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3(0.8f, 0.8f, 0.f))));

	hitables[2] = std::make_shared<Sphere>(Vec3(1.f, 0.f, -1.f), 0.5f,
		std::make_shared<Metal>(Vec3(0.8f, 0.6f, 0.2f), 0.f));

	hitables[3] = std::make_shared<Sphere>(Vec3(-1.f, 0.f, -1.f), 0.5f,
		std::make_shared<Dielectric>(1.5f));

	hitables[4] = std::make_shared<Sphere>(Vec3(-1.f, 0.f, -1.f), -0.45f,
		std::make_shared<Dielectric>(1.2f));

	return BVHNode(hitables, 0.f, 1.f);
}

BVHNode createTwoSpheresScene()
{
	std::vector<std::shared_ptr<Hitable>> hitables;

    std::shared_ptr<Texture> noiseTexture = std::make_shared<NoiseTexture>(2.f);

	// Create a big sphere that will serve as the floor to the scene
	hitables.push_back(std::make_shared<Sphere>(
		Vec3(0.f, -1000.0f, 0.f),
		1000.f,
		std::make_shared<Lambertian>(noiseTexture))
	);

	hitables.push_back(std::make_shared<Sphere>(
		Vec3(0.f, 2.f, 0.f),
		2.f,
		std::make_shared<Lambertian>(noiseTexture))
	);

	return BVHNode(hitables, 0.f, 1.f);
}

BVHNode createRandomScene(RandomGenerator randomGenerator)
{
	std::vector<std::shared_ptr<Hitable>> hitables;

	std::shared_ptr<Texture> checkerTexture = std::make_shared<CheckerTexture>(
		std::make_shared<ConstantTexture>(Vec3(0.1f, 0.1f, 0.1f)),
		std::make_shared<ConstantTexture>(Vec3(0.9f, 0.9f, 0.9f))
	);

	// Create a big sphere that will serve as the floor to the scene
	hitables.push_back(std::make_shared<Sphere>(
		Vec3(0.f, -1000.0f, 0.f),
		1000.f,
		std::make_shared<Lambertian>(checkerTexture))
	);

	for (int a = -20; a < 20; ++a)
	{
		for (int b = -20; b < 20; ++b)
		{
			float chooseMaterial = randomGenerator.nextFloat();
			Vec3 center(a + 0.9f + randomGenerator.nextFloat(), 0.2f, b + 0.9f + randomGenerator.nextFloat());
			if ((center - Vec3(4.f, 0.2f, 0.f)).length() > 0.9f)
			{
				if (chooseMaterial < 0.8f) // diffuse
				{
				#ifdef MOVING_SPHERES
					hitables.push_back(std::make_shared<MovingSphere>(
						center,
						center + Vec3(0.f, 0.5f * randomGenerator.nextFloat(), 0.f),
						0.f, // timeStart
						1.f, // timeEnd
						0.2f, // radius
						std::make_shared<Lambertian>(
							std::make_shared<ConstantTexture>(Vec3(
								randomGenerator.nextFloat(),
								randomGenerator.nextFloat(),
								randomGenerator.nextFloat())))

						));
				#else
					hitables.push_back(std::make_shared<Sphere>(
						center,
						0.2f,
						std::make_shared<Lambertian>(
							std::make_shared<ConstantTexture>(Vec3(
								randomGenerator.nextFloat() * randomGenerator.nextFloat(),
								randomGenerator.nextFloat() * randomGenerator.nextFloat(),
								randomGenerator.nextFloat() * randomGenerator.nextFloat())))
						));
				#endif
				}
				else if (chooseMaterial < 0.95f) // metal
				{
				#ifdef MOVING_SPHERES
					hitables.push_back(std::make_shared<MovingSphere>(
						center,
						center + Vec3(0.f, 0.5f * randomGenerator.nextFloat(), 0.f),
						0.f, // timeStart
						1.f, // timeEnd
						0.2f, // radius
						std::make_shared<Metal>(
							Vec3(
								0.5f * (1.f + randomGenerator.nextFloat()),
								0.5f * (1.f + randomGenerator.nextFloat()),
								0.5f * (1.f + randomGenerator.nextFloat())),
							0.5 * randomGenerator.nextFloat())
						));
				#else
					hitables.push_back(std::make_shared<Sphere>(
						center,
						0.2f,
						std::make_shared<Metal>(
							Vec3(
								0.5f * (1.f + randomGenerator.nextFloat()),
								0.5f * (1.f + randomGenerator.nextFloat()),
								0.5f * (1.f + randomGenerator.nextFloat())),
                            0.5f * randomGenerator.nextFloat())
						));
				#endif
				}
				else // glass
				{
				#ifdef MOVING_SPHERES
					hitables.push_back(std::make_shared<MovingSphere>(
						center,
						center + Vec3(0.f, 0.5f * randomGenerator.nextFloat(), 0.f),
						0.f, // timeStart
						1.f, // timeEnd
						0.2f, // radius
						std::make_shared<Dielectric>(1.f + randomGenerator.nextFloat())
						));
				#else
					hitables.push_back(std::make_shared<Sphere>(
						center,
						0.2f,
						std::make_shared<Dielectric>(1.f + randomGenerator.nextFloat())
						));
				#endif
				}
			}
		}
	}

	hitables.push_back(std::make_shared<Sphere>(
		Vec3(0.f, 1.f, 0.f),
		1.f,
		std::make_shared<Dielectric>(1.8f)));

	hitables.push_back(std::make_shared<Sphere>(
		Vec3(-4.f, 1.f, 0.f),
		1.f,
		std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3(0.8f, 0.2f, 0.1f)))));

	hitables.push_back(std::make_shared<Sphere>(
		Vec3(4.f, 1.f, 0.f),
		1.f,
		std::make_shared<Metal>(Vec3(0.7f, 0.6f, 0.5f), 0.f)));

	return BVHNode(hitables, 0.f, 1.f);
}

int main()
{
	RandomGenerator randomGenerator;

	// Create scene
    auto world = createSimpleLightScene();

	// Image parameters
    const int width = 800;
    const int height = 300;
	const int numChannels = 3;
	std::vector<unsigned char> img(width * height * numChannels);

	// Number of samples per pixel
    const int numSamples = 1400;

	// Camera parameters
    Vec3 lookFrom(-25.f, 5.f, 6.f);
    Vec3 lookAt(2.0f, 3.0f, 0.f);
	float verticalFov = 20.f;
	float focusDistance = 10.f;
	float aperture = 0.f;
	float shutterOpen = 0.f;
	float shutterClose = 1.f;
    Camera camera(lookFrom, lookAt, Vec3(0.f, 1.f, 0.f), verticalFov,
                  static_cast<float>(width) / static_cast<float>(height),
                  aperture, focusDistance, shutterOpen, shutterClose);

	// Iterate over scene space 
	for (int rowIndex = height - 1; rowIndex >= 0; --rowIndex)
	{
		for (int columnIndex = 0; columnIndex < width; ++columnIndex)
		{
			Vec3 color(0.f, 0.f, 0.f);
			for (int sample_index = 0; sample_index < numSamples; ++sample_index)
			{
				float s = (static_cast<float>(columnIndex) + randomGenerator.nextFloat()) / static_cast<float>(width);
				float t = (static_cast<float>(rowIndex) + randomGenerator.nextFloat()) / static_cast<float>(height);
				Ray ray = camera.getRay(s, t);
				color += getColor(ray, world, 0);
			}

			color /= float(numSamples);
			color = Vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));

			int y = height - rowIndex - 1;
			img[(columnIndex + y * width) * numChannels + 0] = (unsigned char)(255.99f * color[0]);
			img[(columnIndex + y * width) * numChannels + 1] = (unsigned char)(255.99f * color[1]);
			img[(columnIndex + y * width) * numChannels + 2] = (unsigned char)(255.99f * color[2]);
		}
	}

    stbi_write_png("simple_light_scene_3_1400samples.png", width, height, numChannels, img.data(), width*numChannels);

	return 0;
}
