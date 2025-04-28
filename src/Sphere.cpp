/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Sphere
*/

#include "Sphere.hpp"

#include <cmath>

#include "Error.hpp"

#define EPSILON 1e-12

namespace RayTracer {

// bool solveQuadratic(const double a, const double b, const double c, double &x0,
//                     double &x1) {
//     double discr = b * b - 4.0 * a * c;
//     if (discr < 0.0) return false;

//     if (discr == 0.0) {
//         x0 = x1 = -0.5 * b / a;
//     } else {
//         double sqrtDiscr = std::sqrt(discr);
//         double q = (b > 0.0) ? -0.5 * (b + sqrtDiscr) : -0.5 * (b - sqrtDiscr);
//         x0 = q / a;
//         x1 = c / q;
//         if (x0 > x1) std::swap(x0, x1);
//     }
//     return true;
// }

// HitRecord Sphere::hit(const Ray &ray) const {
//     Math::Vector3D L = ray.pos - this->pos;
//     double a = ray.dir.dot(ray.dir);
//     double b = 2.0 * ray.dir.dot(L);
//     double c = L.dot(L) - radius * radius;
//     double t0;
//     double t1;

//     if (!solveQuadratic(a, b, c, t0, t1)) return HitRecord();

//     if (t0 < 0.0) {
//         t0 = t1;
//         if (t0 < 0.0) return HitRecord();
//     }

//     return HitRecord(t0, ray, *this, (ray.at(t0) - this->pos) / this->radius);
// }

HitRecord Sphere::hit(const Ray &ray) const {
    Math::Vector3D L = ray.pos - this->pos;
    double a = ray.dir.dot(ray.dir);
    double b = ray.dir.dot(L);
    double c = L.dot(L) - radius * radius;
    double discr = b * b - a * c;

    if (discr < EPSILON) return HitRecord();

    double sqrtDiscr = std::sqrt(discr);
    double t0 = (-b - sqrtDiscr) / a;
    double t1 = (-b + sqrtDiscr) / a;
    double t = (t0 > 0.0) ? t0 : t1;

    if (t < 0.0) return HitRecord();

    return HitRecord(t, ray, *this, (ray.at(t) - this->pos) / this->radius);
}

std::ostream &operator<<(std::ostream &out, const Sphere &sphere) {
    out << "Sphere(pos=" << sphere.pos << ", radius=" << sphere.radius << ")"
        << std::endl;
    return out;
}

}  // namespace RayTracer
