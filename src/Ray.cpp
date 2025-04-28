/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Ray
*/

#include "Ray.hpp"

namespace RayTracer {

std::ostream &operator<<(std::ostream &out, const Ray &ray) {
    out << "Ray(pos=" << ray.pos  << ", dir=" << ray.dir << ")" << std::endl;
    return out;
}

Math::Vector3D Ray::at(double t) const {
    return this->pos + this->dir * t;
}

}  // namespace RayTracer
