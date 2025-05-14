/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** AABB
*/

#ifndef AABB_HPP
#define AABB_HPP

#include "FixCrossInclude.hpp"
#include "HitRecord.hpp"
#include "IShape.hpp"
#include "Interval.hpp"
#include "Ray.hpp"
#include "Vector.hpp"
#include "Material.hpp"

namespace RayTracer {

class AABB : public RayTracer::IShape {
   public:
    Interval x, y, z;

    std::shared_ptr<Material> mat;

    AABB();
    AABB(const Interval &x, const Interval &y, const Interval &z);
    AABB(const Math::Vector3D &a, const Math::Vector3D &b);
    AABB(const AABB &box0, const AABB &box1);

    const Interval &axisInterval(int n) const;
    bool trueHit(const RayTracer::Ray &r, Interval ray_t) const;

    RayTracer::HitRecord hit(const RayTracer::Ray &r, Interval ray_t) const;

    int longestAxis() const;
    AABB boundingBox() const;

   private:
    void padToMinimums();
};

// AABB operator+(const AABB &bbox, const Math::Vector3D &offset) {
//     return AABB(bbox.x + offset.x, bbox.y + offset.y, bbox.z + offset.z);
// }

// AABB operator+(const Math::Vector3D &offset, const AABB &bbox) {
//     return bbox + offset;
// }
}  // namespace RayTracer

#endif
