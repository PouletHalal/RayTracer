/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Camera
*/

#include "Camera.hpp"

namespace RayTracer {

Math::Vector3D sample_square() {
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit
    // square.
    return Math::Vector3D(random_double() - 0.5, random_double() - 0.5, 0);
}

Ray Camera::ray(double u, double v, double screenWidth, double screenHeight) {
    Math::Vector3D offset = sample_square();
    Math::Vector3D pixel_sample = this->screen.pos +
                                  this->screen.bottomSide * (u + (offset.x / screenWidth)) +
                                  this->screen.leftSide * (v + (offset.y / screenHeight));
    
    return Ray(this->pos, pixel_sample - this->pos);
}

void Camera::setPos(Math::Vector3D pos) {
    Math::Vector3D offset = pos - this->pos;

    this->pos += offset;
    this->screen.pos += offset;
}

void Camera::rotateX(double angle) {
    Math::Vector3D toScreen = this->screen.pos - this->pos;

    this->rotationX += angle;
    this->screen.pos -= toScreen;
    toScreen.rotateX(angle);
    this->screen.pos += toScreen;
    this->screen.bottomSide.rotateX(angle);
}

void Camera::rotateY(double angle) {
    Math::Vector3D toScreen = this->screen.pos - this->pos;

    this->rotationY += angle;
    this->screen.pos -= toScreen;
    toScreen.rotateY(angle);
    this->screen.pos += toScreen;
    this->screen.bottomSide.rotateY(angle);
}

void Camera::rotateZ(double angle) {
    Math::Vector3D toScreen = this->screen.pos - this->pos;

    this->rotationZ += angle;
    this->screen.pos -= toScreen;
    toScreen.rotateZ(angle);
    this->screen.pos += toScreen;
    this->screen.bottomSide.rotateZ(angle);
}

void Camera::move(Math::Vector3D offset) {
    this->setPos(this->pos + offset.rotatedX(this->rotationX)
                                 .rotatedY(this->rotationY)
                                 .rotatedZ(this->rotationZ));
}

}  // namespace RayTracer
