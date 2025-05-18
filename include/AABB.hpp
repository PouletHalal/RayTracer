/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** AABB
*/

#ifndef AABB_HPP
#define AABB_HPP

#include <memory>
#include "FixCrossInclude.hpp"
#include "Interval.hpp"
#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/IShape.hpp"
#include "plugins/Material.hpp"

namespace RayTracer {

class LambertianDebug : public Material {
   public:
    LambertianDebug(const Math::Vector3D &albedo);

    bool scatter(const Ray &r_in, const HitRecord &rec,
                 Math::Vector3D &attenuation, Ray &scattered) const override;
    virtual std::unique_ptr<Material> duplicate(void) override;
    void setColor(const Math::Vector3D &) override {};
    void save(libconfig::Setting &) const override {};



   private:
    Math::Vector3D albedo;
};

class AABB : public IShape {
   public:
    Interval x, y, z;

    AABB();
    AABB(const AABB &bbox);
    AABB(const Interval &x, const Interval &y, const Interval &z);
    AABB(const Math::Vector3D &a, const Math::Vector3D &b);
    AABB(const AABB &box0, const AABB &box1);

    AABB &operator=(const RayTracer::AABB &bbox);
    const Interval &axisInterval(int n) const;
    bool trueHit(const Ray &r, Interval ray_t) const;

    HitRecord hits(const Ray &r, Interval ray_t) const override;

    int longestAxis() const;
    void move(const Math::Vector3D &offset) override;
    void rotate(const Math::Vector3D &angles) override;
    void scale(size_t scale) override;
    void setPosition(const Math::Vector3D &newPos) override;
    const AABB &boundingBox() const override;
    void setBoundingBox(const AABB &bbox) override;
    Math::Vector3D getPointColor(const Math::Vector3D &) const override {return {1, 1, 1};};
    void setMaterial(std::unique_ptr<Material> &newMaterial) override;
    std::unique_ptr<Material> &getMaterial() override {return this->material_;};
    void reset(void);
    BVHNode *getBVH(void) const override { return nullptr; };
    void setBVH(BVHNode *) override {};
    void save(libconfig::Setting &) const override {return;};

   private:
    std::unique_ptr<Material> material_;
    void padToMinimums();
};

std::ostream &operator<<(std::ostream &out, const AABB &aabb);

}  // namespace RayTracer

#endif
