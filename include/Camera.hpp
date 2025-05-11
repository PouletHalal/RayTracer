/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Camera
*/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Ray.hpp"
#include "Rectangle.hpp"
#include "Vector.hpp"

namespace RayTracer {

class Camera {
   public:
    Math::Vector3D pos;
    Rectangle screen;
    double rotationX = 0;
    double rotationY = 0;
    double rotationZ = 0;

    Camera() : screen(Math::Vector3D(-0.5, -0.5, -1), Math::Vector3D(1, 0, 0), Math::Vector3D(0, 1, 0)) {};
    Ray ray(double u, double v, double screenWidth, double screenHeight);
    void setPos(Math::Vector3D pos);
    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);
    void move(Math::Vector3D offset);
};

}  // namespace RayTracer

#endif
