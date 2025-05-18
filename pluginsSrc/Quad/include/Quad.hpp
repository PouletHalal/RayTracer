/*
** EPITECH PROJECT, 2025
** Quad
** File description:
** Quad
*/

#ifndef QUAD_HPP
#define QUAD_HPP

#include <libconfig.h++>

#include "Box3D.hpp"
#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/AShape.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {
class Quad : public AShape {
   public:
    Math::Vector3D _origin;
    Math::Vector3D _u;
    Math::Vector3D _v;
    Math::Vector3D _normal;
    Box3D _box;
    std::unique_ptr<Material> material_;

    Quad(const libconfig::Setting &settings);
    Quad(Math::Vector3D q, Math::Vector3D u, Math::Vector3D v);

    void scale(size_t scale) override;
    void move(const Math::Vector3D &offset) override;
    void rotate(const Math::Vector3D &angles) override;
    void setPosition(const Math::Vector3D &newPos) override;
    HitRecord hits(const Ray &ray, Interval ray_t) const override;
    Math::Vector3D getPointColor(const Math::Vector3D &) const override {return {1, 1, 1};};
    void setMaterial(std::unique_ptr<Material> &) override;
    void save(libconfig::Setting &) const override;

};
}  // namespace RayTracer

#endif
