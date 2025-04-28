/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** HitRecord
*/

#ifndef HITRECORD_HPP
#define HITRECORD_HPP

#include "Ray.hpp"
#include "IShape.hpp"
#include "Vector.hpp"

namespace RayTracer {

class IShape; // this fix cross include, trust the process

class HitRecord {
   public:
    Math::Vector3D p;
    Math::Vector3D normal;
    double t = 0;
    bool frontFace = false;
    bool missed = true;

    HitRecord() = default;
    HitRecord(const double t, const Ray &ray, const IShape &shape);
};

}  // namespace RayTracer

#endif
