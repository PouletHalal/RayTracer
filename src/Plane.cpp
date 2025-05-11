/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane
*/

#include "Plane.hpp"

namespace RayTracer {

HitRecord Plane::hit(const Ray &ray) const {
    double denom = orientation.dot(ray.dir);
    if (std::abs(denom) > EPSILON) {
        Math::Vector3D p0l0 = pos - ray.pos;
        double t = p0l0.dot(orientation) / denom;
        if (t >= 0) {
            HitRecord rec = HitRecord(t, ray, *this, this->orientation);
            rec.shape_hit = this;
            return rec;
        }
    }
    return HitRecord();
}

}  // namespace RayTracer