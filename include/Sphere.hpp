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
#include "Material.hpp"

namespace RayTracer {

class Sphere : public AShape {
   public:
    Math::Vector3D pos;
    double radius;
    std::shared_ptr<Material> mat;

    Sphere(Math::Vector3D pos, double radius, std::shared_ptr<Material> mat);
    HitRecord hit(const Ray &ray, Interval interval) const override;
};

std::ostream &operator<<(std::ostream &out, const Sphere &ray);

}  // namespace RayTracer

#endif
