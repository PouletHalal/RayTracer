/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** main
*/

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "BVHNode.hpp"
#include "Camera.hpp"
#include "HitRecord.hpp"
#include "Material.hpp"
#include "Plane.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Utils.hpp"
#include "Vector.hpp"

#define CAM_SPEED 0.03
#define MAX_RAY_BOUNCE 10
#define MAX_RAY 5
#define GRAPHIC true

struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

void setPixelColor(uint8_t *array, int index, rgb color) {
    array[index * 4] = color.r;
    array[index * 4 + 1] = color.g;
    array[index * 4 + 2] = color.b;
    array[index * 4 + 3] = 255;
}

Math::Vector3D getSkyColor(const RayTracer::Ray &r) {
    Math::Vector3D unit_direction = r.dir.normalized();
    double a = 0.5 * (unit_direction.y + 1.0);
    return Math::Vector3D(1, 1, 1) * a +
           Math::Vector3D(0.5, 0.7, 1.0) * (1.0 - a);
}

inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0) return std::sqrt(linear_component);

    return 0;
}

Math::Vector3D trace_ray(const RayTracer::Ray &r, int depth,
                         const RayTracer::Scene &scene) {
    if (depth <= 0) return Math::Vector3D(0, 0, 0);

    RayTracer::HitRecord rec = scene.hit(r, Interval(0, DOUBLE_INFINITY));

    if (rec.missed || rec.t < 1E-10) {
        return getSkyColor(r);
    }

    RayTracer::Ray scattered;
    Math::Vector3D attenuation;
    if (rec.mat->scatter(r, rec, attenuation, scattered))
        return attenuation * trace_ray(scattered, depth - 1, scene);
    return Math::Vector3D(0, 0, 0);
}

uint8_t *generateImage(int width, int height, RayTracer::Camera &cam,
                       RayTracer::Scene &scene, uint8_t *array) {
    double screenWidth = width;
    double screenHeight = height;
    Math::Vector3D vec;

    for (double y = 0; y < screenHeight; y++) {
        for (double x = 0; x < screenWidth; x++) {
            double u = x / screenWidth;
            double v = y / screenHeight;
            RayTracer::Ray r = cam.ray(u, v, screenWidth, screenHeight);
            vec = trace_ray(r, MAX_RAY_BOUNCE, scene);
            vec.x = linear_to_gamma(vec.x);
            vec.y = linear_to_gamma(vec.y);
            vec.z = linear_to_gamma(vec.z);
            setPixelColor(array, y * width + x,
                          {static_cast<unsigned char>(vec.x * 255),
                           static_cast<unsigned char>(vec.y * 255),
                           static_cast<unsigned char>(vec.z * 255)});
        }
        std::cout << y << "/" << height << std::endl;
    }
    return array;
}

int main() {
    int screenWidth = 800;
    int screenHeight = 800;
    sf::Clock clock;
    RayTracer::Camera cam;
#if GRAPHIC
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight),
                            "SFML window");
#endif
    const unsigned int W = 200;
    const unsigned int H = 200;

    long long *image =
        static_cast<long long *>(malloc(sizeof(long long) * (W * H * 4)));
    sf::Uint8 true_image[W * H * 4];
    int nbrRay = 0;
    sf::Uint8 *pixels;
    sf::Texture texture;
    texture.create(W, H);
    sf::Sprite sprite(texture);
    sprite.setScale(screenWidth / W, screenHeight / H);
    cam.setPos({0, -2, 15});

    uint8_t *array =
        static_cast<uint8_t *>(malloc(sizeof(uint8_t) * (W * H * 4)));
    RayTracer::Scene scene;
    auto ground_material =
        std::make_shared<RayTracer::Lambertian>(Math::Vector3D(0.5, 0.5, 0.5));
    scene.addShape(std::make_shared<RayTracer::Sphere>(
        Math::Vector3D(0, 1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            double size = random_double(0.15, 0.4);
            Math::Vector3D center(a + 0.9 * random_double(), -size,
                                  b + 0.9 * random_double());

            if ((center - Math::Vector3D(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<RayTracer::Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo =
                        Math::Vector3D::random() * Math::Vector3D::random();
                    sphere_material =
                        std::make_shared<RayTracer::Lambertian>(albedo);
                    scene.addShape(std::make_shared<RayTracer::Sphere>(
                        center, size, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Math::Vector3D::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material =
                        std::make_shared<RayTracer::Metal>(albedo, fuzz);
                    scene.addShape(std::make_shared<RayTracer::Sphere>(
                        center, size, sphere_material));
                } else {
                    // glass
                    sphere_material =
                        std::make_shared<RayTracer::Dielectric>(1.5);
                    scene.addShape(std::make_shared<RayTracer::Sphere>(
                        center, size, sphere_material));
                }
            }
        }
    }

    // auto material1 = std::make_shared<RayTracer::Dielectric>(1.5);
    // scene.addShape(std::make_shared<RayTracer::Sphere>(Math::Vector3D(0, 1, 0),
    //                                                    1.0, material1));

    // auto material2 =
    //     std::make_shared<RayTracer::Lambertian>(Math::Vector3D(0.4, 0.2, 0.1));
    // scene.addShape(std::make_shared<RayTracer::Sphere>(Math::Vector3D(-4, 1, 0),
    //                                                    1.0, material2));

    // auto material3 =
    //     std::make_shared<RayTracer::Metal>(Math::Vector3D(0.7, 0.6, 0.5), 0.0);
    // scene.addShape(std::make_shared<RayTracer::Sphere>(Math::Vector3D(4, 1, 0),
    //                                                    1.0, material3));

    // scene = RayTracer::Scene(std::make_shared<BVHNode>(scene, 12));

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            for (int i = 0; i < 4; i++) {
                int index = (y * W + x) * 4 + i;
                image[index] = 0;
                true_image[index] = 255;
            }
        }
    }
    int i = 0;
#if GRAPHIC
    while (window.isOpen() && i > -1) {
#else
    while (i > -1) {
#endif
        if (clock.getElapsedTime().asSeconds() < 1.0 / 60.0) continue;
        clock.restart();

        pixels = generateImage(W, H, cam, scene, array);
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                for (int i = 0; i < 3; i++) {
                    int index = (y * W + x) * 4 + i;
                    image[index] += pixels[index];
                    true_image[index] = image[index] / (nbrRay + 1);
                }
            }
        }
        texture.update(true_image);
        nbrRay++;
        if (nbrRay > MAX_RAY) {
            nbrRay = 0;
            for (int y = 0; y < H; y++) {
                for (int x = 0; x < W; x++) {
                    for (int i = 0; i < 4; i++) {
                        int index = (y * W + x) * 4 + i;
                        image[index] = 0;
                    }
                }
            }
        }

#if GRAPHIC
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed) window.close();
#endif

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            cam.move({-CAM_SPEED, 0, 0});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            cam.move({CAM_SPEED, 0, 0});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            cam.move({0, -CAM_SPEED, 0});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            cam.move({0, CAM_SPEED, 0});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            cam.move({0, 0, -CAM_SPEED});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            cam.move({0, 0, CAM_SPEED});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) cam.rotateY(0.1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) cam.rotateY(-0.1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) cam.rotateX(0.1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) cam.rotateX(-0.1);

        std::cout << "frame count: " << nbrRay << std::endl;
#if GRAPHIC
        window.clear();
        window.draw(sprite);
        window.display();
#endif
        i++;
    }

    return EXIT_SUCCESS;
}
