/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Rectangle
*/

#include "Rectangle.hpp"

namespace RayTracer {

Math::Vector3D Rectangle::pointAt(double u, double v) {
    return this->pos + this->bottomSide * u + this->leftSide * v;
}

}  // namespace RayTracer
