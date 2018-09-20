#include "Core/Camera.h"

#include "Hitables/Box.h"
#include "Hitables/Rectangle.h"
#include "Hitables/Sphere.h"
#include "Hitables/MovingSphere.h"
#include "Hitables/HitableList.h"
#include "Hitables/BVHNode.h"
#include "Hitables/Rotate.h"
#include "Hitables/Translate.h"
#include "Hitables/FlipNormals.h"
#include "Hitables/ConstantMedium.h"

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

#include <iostream>

// Template for either BVHNode or HitableList
template <class HitableGroup>
Vec3 getColor(const Ray& ray, HitableGroup& world, int depth)
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
    }
}

HitableList createNextWeekFinalRender()
{
    std::vector<std::shared_ptr<Hitable>> hitables(10);


    std::shared_ptr<Material> white = std::make_shared<Lambertian>(
                                          std::make_shared<ConstantTexture>(
                                              Vec3(0.73f, 0.73f, 0.73f)));

    std::shared_ptr<Material> ground = std::make_shared<Lambertian>(
                                           std::make_shared<ConstantTexture>(
                                                   Vec3(0.12f, 0.45f, 0.15f)));

    // Create ground with randomly sized boxes
    size_t n = 20;
    std::vector<std::shared_ptr<Hitable>> groundBoxes(n*n);
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            float w = 100.f;
            float x0 = -1000.f + static_cast<float>(i)*w;
            float z0 = -1000.f + static_cast<float>(j)*w;
            float y0 = 0.f;
            float x1 = x0 + w;
            float y1 = 100.f * RandomGenerator::randFloat() + 0.01f;
            float z1 = z0 + w;
            groundBoxes[i * n + j] = std::make_shared<Box>(Vec3(x0, y0, z0),
                                                          Vec3(x1, y1, z1),
                                                          ground);
        }
    }

    size_t numHit = 0;
    hitables[numHit++] = std::make_shared<BVHNode>(groundBoxes, 0.f, 1.f);

    // Light
    std::shared_ptr<Material> light = std::make_shared<DiffuseLight>(
                                          std::make_shared<ConstantTexture>(
                                              Vec3(7.f, 7.f, 7.f)));
    hitables[numHit++] = std::make_shared<XZRectangle>(123.f, 423.f, 147.f, 412.f, 554.f, light);

    // Moving sphere
    Vec3 center(400.f, 400.f, 200.f);
    hitables[numHit++] = std::make_shared<MovingSphere>(center, center + Vec3(30.f, 0.f, 0.f), 0.f, 1.f, 50.f,
                                                        std::make_shared<Lambertian>(
                                                            std::make_shared<ConstantTexture>(Vec3(0.7f, 0.3f, 0.1f)))
                                                        );
    // Spheres with different materials

    // Dielectric
    hitables[numHit++] = std::make_shared<Sphere>(
                             Vec3(260.f, 150.f, 45.f),
                             50.f,
                             std::make_shared<Dielectric>(1.5f));

    // Metal
    hitables[numHit++] = std::make_shared<Sphere>(
                             Vec3(0.f, 150.f, 145.f),
                             100.f,
                             std::make_shared<Metal>(Vec3(0.8f, 0.8f, 0.9f), 1.f));

    // Volume
    std::shared_ptr<Hitable> boundary = std::make_shared<Sphere>(
                                            Vec3(360.f, 150.f, 145.f),
                                            70.f,
                                            std::make_shared<Dielectric>(1.5f));
    hitables[numHit++] = boundary;

    hitables[numHit++] = std::make_shared<ConstantMedium>(
                             boundary, 0.2f,
                             std::make_shared<ConstantTexture>(Vec3(0.2f, 0.4f, 0.9f)));

    // Volume surrounding the whole scene
    boundary = std::make_shared<Sphere>(
                   Vec3(0.f, 0.f, 0.f),
                   5000.f,
                   std::make_shared<Dielectric>(1.5f));

    hitables[numHit++] = std::make_shared<ConstantMedium>(
                             boundary, 0.0001f,
                             std::make_shared<ConstantTexture>(Vec3(1.0f, 1.0f, 1.0f)));

    // Noise Texture
    std::shared_ptr<Texture> noiseTexture = std::make_shared<MarbleTexture>(0.1f);
    hitables[numHit++] = std::make_shared<Sphere>(
                             Vec3(220.f, 280.f, 300.f),
                             80.f,
                             std::make_shared<Lambertian>(noiseTexture));


    // Create box with a bunch of sphere instances
    size_t numSpheres = 1000;
    std::vector<std::shared_ptr<Hitable>> boxSpheres(numSpheres);
    for (size_t i = 0; i < numSpheres; ++i)
    {
        boxSpheres[i] = std::make_shared<Sphere>(Vec3(165.f * RandomGenerator::randFloat(),
                                                      165.f * RandomGenerator::randFloat(),
                                                      165.f * RandomGenerator::randFloat()),
                                                 10.f,
                                                 white);
    }
    hitables[numHit++] = std::make_shared<Translate>(
                             std::make_shared<RotateY>(
                                 std::make_shared<BVHNode>(boxSpheres, 0.f, 1.f)
                                 , 15.f),
                             Vec3(-100.f, 270.f, 395.f));

    return HitableList(hitables);
}

BVHNode createSmokeCornellBox()
{
    std::vector<std::shared_ptr<Hitable>> hitables(8);

    std::shared_ptr<Material> red = std::make_shared<Lambertian>(
                                        std::make_shared<ConstantTexture>(
                                            Vec3(0.65f, 0.05f, 0.05f)));
    std::shared_ptr<Material> white = std::make_shared<Lambertian>(
                                          std::make_shared<ConstantTexture>(
                                              Vec3(0.73f, 0.73f, 0.73f)));
    std::shared_ptr<Material> green = std::make_shared<Lambertian>(
                                          std::make_shared<ConstantTexture>(
                                              Vec3(0.12f, 0.45f, 0.15f)));
    std::shared_ptr<Material> light = std::make_shared<DiffuseLight>(
                                          std::make_shared<ConstantTexture>(
                                                  Vec3(7.f, 7.f, 7.f)));
    // Walls
    hitables[0] = std::make_shared<FlipNormals>(std::make_shared<YZRectangle>(0.f, 555.f, 0.f, 555.f, 555.f, green));
    hitables[1] = std::make_shared<YZRectangle>(0.f, 555.f, 0.f, 555.f, 0.f, red);
    hitables[2] = std::make_shared<FlipNormals>(std::make_shared<XZRectangle>(0.f, 555.f, 0.f, 555.f, 555.f, white));
    hitables[3] = std::make_shared<XZRectangle>(0.f, 555.f, 0.f, 555.f, 0.f, white);
    hitables[4] = std::make_shared<FlipNormals>(std::make_shared<XYRectangle>(0.f, 555.f, 0.f, 555.f, 555.f, white));

    // Light
    hitables[5] = std::make_shared<XZRectangle>(113.f, 443.f, 127.f, 432.f, 554.f, light);

    // Boxes
    std::shared_ptr<Hitable> box1 = std::make_shared<Translate>(
                                        std::make_shared<RotateY>(
                                            std::make_shared<Box>(
                                                Vec3(0.f, 0.f, 0.f),
                                                Vec3(165.f, 165.f, 165.f),
                                                white),
                                            -18.f),
                                        Vec3(130.f, 0.f, 65.f));

    std::shared_ptr<Hitable> box2 = std::make_shared<Translate>(
                                        std::make_shared<RotateY>(
                                            std::make_shared<Box>(
                                                Vec3(0.f, 0.f, 0.f),
                                                Vec3(165.f, 330.f, 165.f),
                                                white),
                                            15.f),
                                        Vec3(265.f, 0.f, 295.f));

    hitables[6] = std::make_shared<ConstantMedium>(
                      box1, 0.01f, std::make_shared<ConstantTexture>(Vec3(1.f, 1.f, 1.f)));

    hitables[7] = std::make_shared<ConstantMedium>(
                      box2, 0.01f, std::make_shared<ConstantTexture>(Vec3(0.f, 0.f, 0.f)));

    return BVHNode(hitables, 0.f, 1.f);
}

BVHNode createCornellBox()
{
    std::vector<std::shared_ptr<Hitable>> hitables(8);

    std::shared_ptr<Material> red = std::make_shared<Lambertian>(
                                        std::make_shared<ConstantTexture>(
                                            Vec3(0.65f, 0.05f, 0.05f)));
    std::shared_ptr<Material> white = std::make_shared<Lambertian>(
                                          std::make_shared<ConstantTexture>(
                                              Vec3(0.73f, 0.73f, 0.73f)));
    std::shared_ptr<Material> green = std::make_shared<Lambertian>(
                                          std::make_shared<ConstantTexture>(
                                              Vec3(0.12f, 0.45f, 0.15f)));
    std::shared_ptr<Material> light = std::make_shared<DiffuseLight>(
                                          std::make_shared<ConstantTexture>(
                                                  Vec3(15.f, 15.f, 15.f)));
    // Walls
    hitables[0] = std::make_shared<FlipNormals>(std::make_shared<YZRectangle>(0.f, 555.f, 0.f, 555.f, 555.f, green));
    hitables[1] = std::make_shared<YZRectangle>(0.f, 555.f, 0.f, 555.f, 0.f, red);
    hitables[2] = std::make_shared<FlipNormals>(std::make_shared<XZRectangle>(0.f, 555.f, 0.f, 555.f, 555.f, white));
    hitables[3] = std::make_shared<XZRectangle>(0.f, 555.f, 0.f, 555.f, 0.f, white);
    hitables[4] = std::make_shared<FlipNormals>(std::make_shared<XYRectangle>(0.f, 555.f, 0.f, 555.f, 555.f, white));

    // Light
    hitables[5] = std::make_shared<XZRectangle>(213.f, 343.f, 227.f, 332.f, 554.f, light);

    // Boxes
    hitables[6] = std::make_shared<Translate>(std::make_shared<RotateY>(
                                                  std::make_shared<Box>(
                                                      Vec3(0.f, 0.f, 0.f),
                                                      Vec3(165.f, 165.f, 165.f),
                                                      white),
                                                  -18.f),
                                              Vec3(130.f, 0.f, 65.f));
    hitables[7] = std::make_shared<Translate>(std::make_shared<RotateY>(
                                                  std::make_shared<Box>(
                                                      Vec3(0.f, 0.f, 0.f),
                                                      Vec3(165.f, 330.f, 165.f),
                                                      white),
                                                  15.f),
                                              Vec3(265.f, 0.f, 295.f));
    return BVHNode(hitables, 0.f, 1.f);
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

    hitables[3] = std::make_shared<XYRectangle>(-2.f, 2.f, 1.f, 4.f, -4.f,
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
    hitables.reserve(2);

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

BVHNode createRandomScene()
{
    int n = 20;
    std::vector<std::shared_ptr<Hitable>> hitables;

    // Reserve the number of hitables that we will be creating
    hitables.reserve((2 * n) * (2 * n) + 4);

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

    for (int a = -n; a < n; ++a)
    {
        for (int b = -n; b < n; ++b)
        {
            float chooseMaterial = RandomGenerator::randFloat();
            Vec3 center(a + 0.9f + RandomGenerator::randFloat(), 0.2f, b + 0.9f + RandomGenerator::randFloat());
            if ((center - Vec3(4.f, 0.2f, 0.f)).length() > 0.9f)
            {
                if (chooseMaterial < 0.8f) // diffuse
                {
                    hitables.push_back(std::make_shared<Sphere>(
                                           center,
                                           0.2f,
                                           std::make_shared<Lambertian>(
                                               std::make_shared<ConstantTexture>(Vec3(
                                                                                     RandomGenerator::randFloat() * RandomGenerator::randFloat(),
                                                                                     RandomGenerator::randFloat() * RandomGenerator::randFloat(),
                                                                                     RandomGenerator::randFloat() * RandomGenerator::randFloat())))
                                           ));
                }
                else if (chooseMaterial < 0.95f) // metal
                {
                    hitables.push_back(std::make_shared<Sphere>(
                                           center,
                                           0.2f,
                                           std::make_shared<Metal>(
                                               Vec3(
                                                   0.5f * (1.f + RandomGenerator::randFloat()),
                                                   0.5f * (1.f + RandomGenerator::randFloat()),
                                                   0.5f * (1.f + RandomGenerator::randFloat())),
                                               0.5f * RandomGenerator::randFloat())
                                           ));
                }
                else // glass
                {
                    hitables.push_back(std::make_shared<Sphere>(
                                           center,
                                           0.2f,
                                           std::make_shared<Dielectric>(1.f + RandomGenerator::randFloat())
                                           ));
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
    // Create scene
    auto world = createNextWeekFinalRender();

    // Image parameters
    const int width = 200;
    const int height = 200;
    const int numChannels = 3;
    std::vector<unsigned char> img(width * height * numChannels);

    // Number of samples per pixel
    const int numSamples = 1000;

    // Camera parameters
    Vec3 lookFrom(478.f, 278.f, -600.f);
    Vec3 lookAt(278.f, 278.f, 0.f);
    Vec3 viewUp(0.f, 1.f, 0.f);
    float verticalFov = 40.f;
    float focusDistance = 10.f;
    float aperture = 0.f;
    float shutterOpen = 0.f;
    float shutterClose = 1.f;
    Camera camera(lookFrom, lookAt, viewUp, verticalFov,
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
                float s = (static_cast<float>(columnIndex) + RandomGenerator::randFloat()) / static_cast<float>(width);
                float t = (static_cast<float>(rowIndex) + RandomGenerator::randFloat()) / static_cast<float>(height);
                Ray ray = camera.getRay(s, t);
                color += getColor(ray, world, 0);
            }

            color /= float(numSamples);
            color = Vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));

            int y = height - rowIndex - 1;
            img[(columnIndex + y * width) * numChannels + 0] = static_cast<unsigned char>(255.99f * color[0]);
            img[(columnIndex + y * width) * numChannels + 1] = static_cast<unsigned char>(255.99f * color[1]);
            img[(columnIndex + y * width) * numChannels + 2] = static_cast<unsigned char>(255.99f * color[2]);
        }
    }

    stbi_write_png("AmyWilliamsBVH.png", width, height, numChannels, img.data(), width*numChannels);

    return 0;
}
