/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle
*/

#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

#include <libconfig.h++>

#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/AShape.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

class Triangle : public AShape {
 public:
  Math::Vector3D a;
  Math::Vector3D b;
  Math::Vector3D c;

  Triangle() = default;
  ~Triangle() = default;
  Triangle(Math::Vector3D a, Math::Vector3D b, Math::Vector3D c)
      : a(a), b(b), c(c) {
    this->bbox = AABB(AABB(this->a, this->b), AABB(this->c, this->c));
  };
  Triangle(const libconfig::Setting &settings);
  HitRecord hits(const Ray &, Interval ray_t) const override;
  void move(const Math::Vector3D &pos) override;
  void rotate(const Math::Vector3D &) override;
  void scale(size_t scale) override;
  void setPosition(const Math::Vector3D &pos) override;
  Math::Vector3D getPointColor(const Math::Vector3D &) const override;
  void save(libconfig::Setting &parent) const override;
};
}  // namespace RayTracer

#endif /* !TRIANGLE_HPP_ */
