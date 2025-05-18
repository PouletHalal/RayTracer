/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Circle
*/

#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <iostream>
#include <libconfig.h++>

#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/AShape.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

class Plane : public AShape {
 public:
  Plane() = default;
  Plane(Math::Vector3D pos, Math::Vector3D orientation);
  Plane(const libconfig::Setting &);
  HitRecord hits(const Ray &ray, Interval ray_t) const override;
  void move(const Math::Vector3D &offset) override;
  void rotate(const Math::Vector3D &angles) override;
  void scale(size_t scale) override;
  void setPosition(const Math::Vector3D &newPos) override;
  void save(libconfig::Setting &parent) const override;
  Math::Vector3D getPointColor(const Math::Vector3D &) const override;

  Math::Vector3D pos_;
  Math::Vector3D orientation_;
};

std::ostream &operator<<(std::ostream &os, const Plane &);


}  // namespace RayTracer

#endif
