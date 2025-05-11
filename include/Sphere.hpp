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
    Math::Vector3D emittance;
    std::shared_ptr<Material> mat;

    Sphere(Math::Vector3D pos, double radius, std::shared_ptr<Material> mat, Math::Vector3D emittance = Math::Vector3D(0, 0, 0)) : pos(pos), radius(radius), mat(mat), emittance(emittance) {};
    HitRecord hit(const Ray &ray) const override;
    Math::Vector3D getEmmitance() const override { return this->emittance; };
};

std::ostream &operator<<(std::ostream &out, const Sphere &ray);

}  // namespace RayTracer

#endif
