/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane
*/

#ifndef PLANE_HPP_
#define PLANE_HPP_

#include <iostream>

#include "AShape.hpp"
#include "Ray.hpp"
#include "Vector.hpp"

namespace RayTracer {

class Plane : public AShape {
   public:
    Plane(Math::Vector3D pos, Math::Vector3D orientation) : pos(pos), orientation(orientation) {};
    HitRecord hit(const Ray &ray) const override;
    virtual Math::Vector3D getEmmitance() const override { return this->emittance; };

    Math::Vector3D pos;
    Math::Vector3D orientation;
    Math::Vector3D emittance;
   protected:
   private:
};

}  // namespace RayTracer
#endif /* !PLANE_HPP_ */
