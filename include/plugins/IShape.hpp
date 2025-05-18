/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** IShape
*/

#ifndef ISHAPE_HPP
#define ISHAPE_HPP

#include <memory>

#include "Interval.hpp"
#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/IObject.hpp"
#include "plugins/Material.hpp"

namespace RayTracer {

class HitRecord;
class AABB;
class BVHNode;

class IShape : public IObject {
   public:
    virtual HitRecord hits(const Ray &ray, Interval ray_t) const = 0;
    virtual Math::Vector3D getPointColor(const Math::Vector3D &point) const = 0;
    virtual const AABB &boundingBox() const = 0;
    virtual void setMaterial(std::unique_ptr<Material> &) = 0;
    virtual std::unique_ptr<Material> &getMaterial() = 0;
    virtual BVHNode *getBVH() const = 0;
    virtual void setBVH(BVHNode *bvhNode) = 0;
    virtual void setBoundingBox(const AABB &bbox) = 0;
    virtual void save(libconfig::Setting &parent) const = 0;
};

};  // namespace RayTracer

#include "AABB.hpp"
#include "BVHNode.hpp"

// #include "AABB.hpp"  // don't move this, trust the process (cross include)

namespace RayTracer {

class HitRecord {
   public:
    bool missed = true;
    double t = 0;
    bool frontFace = false;
    Math::Vector3D normal;
    Math::Vector3D p;
    const IShape *shapeHit;
    const Material *mat;

    HitRecord() = default;
    HitRecord(const double t, const Ray &ray, const IShape &shape,
              const Math::Vector3D &normal,
              const std::unique_ptr<Material> &mat);
};

std::ostream &operator<<(std::ostream &out, const HitRecord &vec);

};  // namespace RayTracer

#endif
