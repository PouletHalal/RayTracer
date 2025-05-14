/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle
*/

#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

#include "AShape.hpp"

namespace RayTracer {

class Triangle : public AShape {
   public:
    Math::Vector3D a;
    Math::Vector3D b;
    Math::Vector3D c;

    Triangle() = default;
    ~Triangle() = default;
    Triangle(Math::Vector3D a, Math::Vector3D b, Math::Vector3D c) : a(a), b(b), c(c) {};
    HitRecord hit(const Ray &ray, Interval interval) const override;

   protected:
   private:
};
}  // namespace RayTracer

#endif /* !TRIANGLE_HPP_ */
