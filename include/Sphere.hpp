/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Circle
*/

#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <iostream>

#include "AShape.hpp"
#include "Ray.hpp"
#include "Vector.hpp"

namespace RayTracer {

class Sphere : public AShape {
   public:
    Math::Vector3D pos;
    double radius;

    Sphere() : radius(0) {};
    Sphere(Math::Vector3D pos, double radius) : pos(pos), radius(radius) {};
    HitRecord hit(const Ray &ray) const override;
    Math::Vector3D normal(const Math::Vector3D &hitPoint) const override;
};

std::ostream &operator<<(std::ostream &out, const Sphere &ray);

}  // namespace RayTracer

#endif
