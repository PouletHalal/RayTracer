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
#include "Sphere.hpp"
#include "Vector.hpp"
#include "Utils.hpp"
#include "HitRecord.hpp"

#define CAM_SPEED 0.03

struct rgba {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

void setPixelColor(uint8_t *array, int index, rgba color) {
    array[index * 4] = color.r;
    array[index * 4 + 1] = color.g;
    array[index * 4 + 2] = color.b;
    array[index * 4 + 3] = color.a;
}

Math::Vector3D getSkyColor(const RayTracer::Ray &r) {
    Math::Vector3D unit_direction = r.dir.normalized();
    double a = 0.5 * (unit_direction.y + 1.0);
    return Math::Vector3D(0.9, 0.9, 0.9) * (1.0 - a) +
           Math::Vector3D(0.3, 0.5, 1.0) * a;
}

uint8_t *generateImage(int width, int height, RayTracer::Camera &cam) {
    uint8_t *array =
        static_cast<uint8_t *>(malloc(sizeof(uint8_t) * (width * height * 4)));
    RayTracer::Sphere s(Math::Vector3D(0, 0, -2), 0.5);
    double screenWidth = width;
    double screenHeight = height;
    Math::Vector3D vec;

    for (double y = 0; y < screenHeight; y++) {
        for (double x = 0; x < screenWidth; x++) {
            double u = x / screenWidth;
            double v = y / screenHeight;
            RayTracer::Ray r = cam.ray(u, v);
            RayTracer::HitRecord hitRecord = s.hit(r);
            
            vec = getSkyColor(r);
            setPixelColor(array, y * width + x,
                          {static_cast<unsigned char>(vec.x * 255),
                           static_cast<unsigned char>(vec.y * 255),
                           static_cast<unsigned char>(vec.z * 255), 255});
            if (!hitRecord.missed) {
                vec = (hitRecord.normal + 1.0) / 2.0;
                setPixelColor(array, y * width + x,
                              {static_cast<unsigned char>(vec.x * 255),
                               static_cast<unsigned char>(vec.y * 255),
                               static_cast<unsigned char>(vec.z * 255), 255});
            } else {
                // setPixelColor(array, y * width + x, {0, 0, 0, 255});
            }
        }
    }
    return array;
}

int main() {
    int screenWidth = 800;
    int screenHeight = 800;
    sf::Clock clock;
    RayTracer::Camera cam;
    Math::Vector3D camPos = {0, 0, 0};
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight),
                            "SFML window");

    const unsigned int W = 800;
    const unsigned int H = 800;

    sf::Uint8 *pixels;
    sf::Texture texture;
    texture.create(W, H);
    sf::Sprite sprite(texture);
    sprite.setScale(screenWidth / W, screenHeight / H);

    while (window.isOpen()) {
        if (clock.getElapsedTime().asSeconds() < 1.0 / 60.0) continue;
        clock.restart();

        pixels = generateImage(W, H, cam);
        texture.update(pixels);

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

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return EXIT_SUCCESS;
}
