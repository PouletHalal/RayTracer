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

namespace RayTracer {

class Sphere : public AShape {
 public:
  Sphere();
  Sphere(Math::Vector3D pos, double radius);
  Sphere(const libconfig::Setting &);
  HitRecord hits(const Ray &ray, Interval interval) const override;
  void move(const Math::Vector3D &offset) override;
  void rotate(const Math::Vector3D &angles) override;
  void scale(size_t scale) override;
  void setPosition(const Math::Vector3D &newPos) override;
  Math::Vector3D getPointColor(const Math::Vector3D &) const override;
  void save(libconfig::Setting &parent) const override;

  Math::Vector3D pos;
  double radius;
};

std::ostream &operator<<(std::ostream &out, const Sphere &ray);

}  // namespace RayTracer

#endif
