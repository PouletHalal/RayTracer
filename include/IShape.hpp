/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** IShape
*/

#ifndef ISHAPE_HPP
#define ISHAPE_HPP

#include "Ray.hpp"
#include "Vector.hpp"
#include "HitRecord.hpp"

namespace RayTracer {

class HitRecord; // this fix cross include, trust the process

class IShape {
   public:
    virtual HitRecord hit(const Ray &ray) const = 0;
    virtual Math::Vector3D normal(const Math::Vector3D &hitPoint) const = 0;
};

}  // namespace RayTracer

#endif
