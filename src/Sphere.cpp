/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Sphere
*/

#include "Sphere.hpp"

#include <cmath>

#include "Error.hpp"

namespace RayTracer {

bool solveQuadratic(const double &a, const double &b, const double &c,
                    double &x0, double &x1) {
    double discr = b * b - 4 * a * c;
    double q;

    if (discr < 0) return false;
    if (discr == 0) {
        x0 = x1 = -0.5 * b / a;
    } else {
        if (b > 0)
            q = -0.5 * (b + std::sqrt(discr));
        else
            q = -0.5 * (b - std::sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);
    return true;
}

// TODO : maybe change it to return a boolean, and pass hitRecord as argument
HitRecord Sphere::hit(const Ray &ray) const {
    double t0;
    double t1;
    Math::Vector3D L = ray.pos - this->pos;
    double a = ray.dir.dot(ray.dir);
    double b = 2 * ray.dir.dot(L);
    double c = L.dot(L) - radius * radius;

    if (!solveQuadratic(a, b, c, t0, t1)) return HitRecord();
    if (t0 > t1) std::swap(t0, t1);
    if (t0 < 0) {
        t0 = t1;
        if (t0 < 0) return HitRecord();
    }
    return HitRecord(t0, ray, *this);
}

Math::Vector3D Sphere::normal(const Math::Vector3D &hitPoint) const {
    return (hitPoint - this->pos) / this->radius;
}

std::ostream &operator<<(std::ostream &out, const Sphere &sphere) {
    out << "Sphere(pos=" << sphere.pos << ", radius=" << sphere.radius << ")"
        << std::endl;
    return out;
}

}  // namespace RayTracer
