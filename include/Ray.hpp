/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Ray
*/

#ifndef RAY_HPP
#define RAY_HPP

#include <iostream>

#include "Vector.hpp"

namespace RayTracer {

class Ray {
   public:
    Math::Vector3D pos;
    Math::Vector3D dir;

    Ray() {};
    Ray(Math::Vector3D pos, Math::Vector3D dir) : pos(pos), dir(dir) {};
    Math::Vector3D at(double t) const;
};

std::ostream &operator<<(std::ostream &out, const Ray &ray);

}  // namespace RayTracer

#endif
