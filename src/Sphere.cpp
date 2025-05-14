/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Sphere
*/

#include "Sphere.hpp"

#include <cmath>

#include "Error.hpp"
#include "Utils.hpp"

namespace RayTracer {

Sphere::Sphere(Math::Vector3D pos, double radius, std::shared_ptr<Material> mat)
    : pos(pos), radius(radius), mat(mat) {
    Math::Vector3D rvec = Math::Vector3D(radius, radius, radius);
    this->bbox = AABB(pos - (rvec), pos + (rvec));
};

HitRecord Sphere::hit(const Ray &ray, Interval interval) const {
    // return this->bbox.hit(ray, interval);
    Math::Vector3D oc = this->pos - ray.pos;
    double a = ray.dir.lengthSquared();
    double h = ray.dir.dot(oc);
    double c = oc.lengthSquared() - radius * radius;

    double discriminant = h * h - a * c;
    if (discriminant < 0) return HitRecord();

    double sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (h - sqrtd) / a;
    if (root <= interval.min || interval.max <= root) {
        root = (h + sqrtd) / a;
        if (root <= interval.min || interval.max <= root) return HitRecord();
    }
    HitRecord rec(root, ray, *this, (ray.at(root) - this->pos) / radius);

    rec.mat = this->mat;
    return rec;
}

std::ostream &operator<<(std::ostream &out, const Sphere &sphere) {
    out << "Sphere(pos=" << sphere.pos << ", radius=" << sphere.radius << ")"
        << std::endl;
    return out;
}

}  // namespace RayTracer
