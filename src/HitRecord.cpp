/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** HitRecord
*/

#include "HitRecord.hpp"

namespace RayTracer {

HitRecord::HitRecord(const double t, const Ray &ray, const IShape &shape,
                     const Math::Vector3D &normal) {
    this->missed = false;
    this->t = t;
    this->p = ray.at(t);
    this->frontFace = ray.dir.dot(normal) < 0;
    this->normal = this->frontFace ? normal : -normal;
}

}  // namespace RayTracer
