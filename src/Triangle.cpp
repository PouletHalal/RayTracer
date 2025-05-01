/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle
*/

#include "Triangle.hpp"

namespace RayTracer {

HitRecord Triangle::hit(const Ray &ray) const {
    Math::Vector3D ab = b - a;
    Math::Vector3D ac = c - a;
    Math::Vector3D n = ab.cross(ac);

    double dot = n.dot(ray.dir);
    if (fabs(dot) < EPSILON) return HitRecord();
    double d = -n.dot(a);
    double t = -(n.dot(ray.pos) + d) / dot;

    if (t < 0) return HitRecord();
    Math::Vector3D p = ray.pos + ray.dir * t;
    Math::Vector3D ne;

    Math::Vector3D ap = p - a;
    ne = ab.cross(ap);
    if (n.dot(ne) < 0)
        return HitRecord();

    Math::Vector3D bc = c - b;
    Math::Vector3D bp = p - b;
    ne = bc.cross(bp);
    if (n.dot(ne) < 0) return HitRecord();
 
	Math::Vector3D ca = a - c;
    Math::Vector3D cp = p - c;
    ne = ca.cross(cp);
    if (n.dot(ne) < 0) return HitRecord();

    return HitRecord(t, ray, *this, n.normalized());
}
}  // namespace RayTracer
