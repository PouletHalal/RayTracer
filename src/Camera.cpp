/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Camera
*/

#include "Camera.hpp"

namespace RayTracer {

Ray Camera::ray(double u, double v) {
    return Ray(this->pos, this->screen.pointAt(u, v) - this->pos);
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
