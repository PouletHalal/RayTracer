/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Rectangle
*/

#include "Raytracer/Rectangle.hpp"

namespace RayTracer {

Rectangle::Rectangle(Math::Vector3D pos, Math::Vector3D bottomSide,
                     Math::Vector3D leftSide)
    : pos(pos), bottomSide(bottomSide), leftSide(leftSide) {};

Math::Vector3D Rectangle::pointAt(double u, double v) {
    return this->pos + this->bottomSide * u + this->leftSide * v;
}

}  // namespace RayTracer
