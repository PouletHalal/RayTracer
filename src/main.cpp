/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** main
*/

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Camera.hpp"
#include "HitRecord.hpp"
#include "Material.hpp"
#include "Plane.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Utils.hpp"
#include "Vector.hpp"

#define CAM_SPEED 0.03
#define MAX_RAY_BOUNCE 50
#define MAX_RAY 1000

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

    RayTracer::HitRecord rec = scene.hit(r);

    if (rec.missed || rec.t < 1E-3) {
        return getSkyColor(r);
    }
    RayTracer::Ray scattered;
    Math::Vector3D attenuation;
    if (rec.mat->scatter(r, rec, attenuation, scattered))
        return attenuation * trace_ray(scattered, depth - 1, scene);
    return Math::Vector3D(0, 0, 0);

    // Math::Vector3D emittance = rec.shape_hit->getEmmitance();
    // Math::Vector3D newDir = rec.normal +
    // Math::Vector3D::random_unit_vector(); RayTracer::Ray newRay =
    //     RayTracer::Ray(rec.p + rec.normal * EPSILON, newDir);

    // Math::Vector3D reflectance = Math::Vector3D(0.9, 0.9, 0.9);
    // // Compute the BRDF for this ray (assuming Lambertian reflection)
    // float cos_theta = newDir.dot(rec.normal);

    // Math::Vector3D incoming = trace_ray(newRay, depth - 1, scene);

    // // Apply the Rendering Equation here.
    // Math::Vector3D color = emittance + (incoming * 0.9 * reflectance);

    // if (color.x > 1) color.x = 1;
    // if (color.y > 1) color.y = 1;
    // if (color.z > 1) color.z = 1;
    // return color;
}

uint8_t *generateImage(int width, int height, RayTracer::Camera &cam) {
    uint8_t *array =
        static_cast<uint8_t *>(malloc(sizeof(uint8_t) * (width * height * 4)));
    double screenWidth = width;
    double screenHeight = height;
    Math::Vector3D vec;
    RayTracer::Scene scene;
    RayTracer::HitRecord hitRecord;
    std::shared_ptr<RayTracer::Material> material_ground =
        std::make_shared<RayTracer::Lambertian>(Math::Vector3D(0.8, 0.8, 0.0));
    std::shared_ptr<RayTracer::Material> material_center =
        std::make_shared<RayTracer::Lambertian>(Math::Vector3D(0.1, 0.2, 0.5));
    std::shared_ptr<RayTracer::Material> material_left =
        std::make_shared<RayTracer::Dielectric>(1.50);
    std::shared_ptr<RayTracer::Material> material_bubble =
        std::make_shared<RayTracer::Dielectric>(1.00 / 1.50);
    std::shared_ptr<RayTracer::Material> material_right =
        std::make_shared<RayTracer::Metal>(Math::Vector3D(0.8, 0.6, 0.2), 1.0);

    // scene.addShape(std::make_shared<RayTracer::Sphere>(
    //     Math::Vector3D(0, -4, -1), 3, Math::Vector3D(0, 0, 1)));
    scene.addShape(std::make_shared<RayTracer::Sphere>(
        Math::Vector3D(0.0, 100.5, -1.0), 100.0, material_ground));
    scene.addShape(std::make_shared<RayTracer::Sphere>(
        Math::Vector3D(0.0, 0.0, -1.2), 0.5, material_center));
    scene.addShape(std::make_shared<RayTracer::Sphere>(
        Math::Vector3D(-1.0, 0.0, -1.0), 0.5, material_left));
    scene.addShape(std::make_shared<RayTracer::Sphere>(
        Math::Vector3D(-1.0, 0.0, -1.0), 0.4, material_bubble));
    scene.addShape(std::make_shared<RayTracer::Sphere>(
        Math::Vector3D(1.0, 0.0, -1.0), 0.5, material_right));
    // scene.addShape(
    //     std::make_shared<RayTracer::Sphere>(Math::Vector3D(0, 0, -4), 0.5,
    //     Math::Vector3D(1, 0.5, 0)));
    // scene.addShape(
    //     std::make_shared<RayTracer::Sphere>(Math::Vector3D(0, 0, -5), 0.5));
    // scene.addShape(std::make_shared<RayTracer::Plane>(
    //     Math::Vector3D(0, 0, -5.5), Math::Vector3D(0, 0, -1)));
    // scene.addShape(std::make_shared<RayTracer::Plane>(
    //     Math::Vector3D(0, 0, 3.5), Math::Vector3D(0, 0, 1)));
    // scene.addShape(std::make_shared<RayTracer::Plane>(
    //     Math::Vector3D(3.5, 0, 0), Math::Vector3D(1, 0, 0)));
    // scene.addShape(std::make_shared<RayTracer::Plane>(
    //     Math::Vector3D(-3.5, 0, 0), Math::Vector3D(-1, 0, 0)));
    for (double y = 0; y < screenHeight; y++) {
        for (double x = 0; x < screenWidth; x++) {
            double u = x / screenWidth;
            double v = y / screenHeight;
            RayTracer::Ray r = cam.ray(u, v, screenWidth, screenHeight);

            hitRecord = scene.hit(r);
            vec = trace_ray(r, MAX_RAY_BOUNCE, scene);
            vec.x = linear_to_gamma(vec.x);
            vec.y = linear_to_gamma(vec.y);
            vec.z = linear_to_gamma(vec.z);

            setPixelColor(array, y * width + x,
                          {static_cast<unsigned char>(vec.x * 255),
                           static_cast<unsigned char>(vec.y * 255),
                           static_cast<unsigned char>(vec.z * 255)});
        }
    }
    return array;
}

int main() {
    int screenWidth = 800;
    int screenHeight = 800;
    sf::Clock clock;
    RayTracer::Camera cam;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight),
                            "SFML window");

    const unsigned int W = 800;
    const unsigned int H = 800;

    long long *image =
        static_cast<long long *>(malloc(sizeof(long long) * (W * H * 4)));
    sf::Uint8 true_image[W * H * 4];
    int nbrRay = 0;
    sf::Uint8 *pixels;
    sf::Texture texture;
    texture.create(W, H);
    sf::Sprite sprite(texture);
    sprite.setScale(screenWidth / W, screenHeight / H);
    cam.setPos({0, -0.25, 2});
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            for (int i = 0; i < 4; i++) {
                int index = (y * W + x) * 4 + i;
                image[index] = 0;
                true_image[index] = 255;
            }
        }
    }
    while (window.isOpen()) {
        if (clock.getElapsedTime().asSeconds() < 1.0 / 60.0) continue;
        clock.restart();

        pixels = generateImage(W, H, cam);
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                for (int i = 0; i < 3; i++) {
                    int index = (y * W + x) * 4 + i;
                    image[index] += pixels[index];
                    true_image[index] = image[index] / (nbrRay + 1);
                }
            }
        }
        free(pixels);
        nbrRay++;
        if (nbrRay > MAX_RAY) nbrRay = 0;
        texture.update(true_image);

        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed) window.close();

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
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return EXIT_SUCCESS;
}
