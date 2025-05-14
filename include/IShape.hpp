/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** IShape
*/

#ifndef ISHAPE_HPP
#define ISHAPE_HPP

#include <memory>

#include "FixCrossInclude.hpp"
#include "HitRecord.hpp"
#include "Interval.hpp"
#include "Ray.hpp"
#include "Vector.hpp"

namespace RayTracer {

class IShape {
   public:
    int depth = 0;
    virtual HitRecord hit(const Ray &ray, Interval infinity) const = 0;
    virtual AABB boundingBox() const = 0;
};

}  // namespace RayTracer

#include "AABB.hpp"  // don't move this, trust the process (cross include)

#endif
