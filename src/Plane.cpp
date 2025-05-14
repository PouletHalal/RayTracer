/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane
*/

#include "Plane.hpp"

namespace RayTracer {

HitRecord Plane::hit(const Ray &ray, Interval interval) const {
    double denom = orientation.dot(ray.dir);

    if (std::abs(denom) > EPSILON) return HitRecord();

    Math::Vector3D p0l0 = pos - ray.pos;
    double t = p0l0.dot(orientation) / denom;

    if (t <= interval.min || interval.max <= t) return HitRecord();
    
    HitRecord rec = HitRecord(t, ray, *this, this->orientation);
    rec.shape_hit = this;
    return rec;
}

}  // namespace RayTracer
