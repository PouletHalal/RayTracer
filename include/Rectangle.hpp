/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Rectangle
*/

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Vector.hpp"

namespace RayTracer {

class Rectangle {
   public:
    Math::Vector3D pos;
    Math::Vector3D bottomSide;
    Math::Vector3D leftSide;
    Rectangle() {};
    Rectangle(Math::Vector3D pos, Math::Vector3D bottomSide, Math::Vector3D leftSide) : pos(pos), bottomSide(bottomSide), leftSide(leftSide) {};
    Math::Vector3D pointAt(double u, double v);
};

}  // namespace RayTracer

#endif
