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

// HitRecord Sphere::hit(const Ray &ray) const {
//     Math::Vector3D L = ray.pos - this->pos;
//     double a = ray.dir.dot(ray.dir);
//     double b = ray.dir.dot(L);
//     double c = L.dot(L) - radius * radius;
//     double discr = b * b - a * c;

//     if (discr < EPSILON) return HitRecord();

//     double sqrtDiscr = std::sqrt(discr);
//     double t0 = (-b - sqrtDiscr) / a;
//     double t1 = (-b + sqrtDiscr) / a;
//     double t = (t0 > 0.0) ? t0 : t1;

//     if (t < 0.0) return HitRecord();

//     HitRecord rec = HitRecord(t, ray, *this, (ray.at(t) - this->pos) /
//     this->radius); rec.mat = this->mat; return rec;
// }

HitRecord Sphere::hit(const Ray &ray) const {
    Math::Vector3D oc = this->pos - ray.pos;
    double a = ray.dir.lengthSquared();
    double h = ray.dir.dot(oc);
    double c = oc.lengthSquared() - radius * radius;

    double discriminant = h * h - a * c;
    if (discriminant < 0) return HitRecord();

    double sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (h - sqrtd) / a;
    if (root <= 1E-4 || INFINITY <= root) {
        root = (h + sqrtd) / a;
        if (root <= 1E-4 || INFINITY <= root) return HitRecord();
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
