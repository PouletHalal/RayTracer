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

#include "plugins/IShape.hpp"
#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"

namespace RayTracer {

class Cylinder : public IShape {
    public:
    Cylinder(const libconfig::Setting &);
    HitRecord hits(const Ray &ray) const override;
    void move(const Math::Vector3D &offset) override;
    void rotate(const Math::Vector3D &angles) override;
    void scale(size_t scale) override;
    void setPosition(const Math::Vector3D &newPos) override;
    void save(libconfig::Setting &parent) const override;

    private:
    HitRecord hitsCapOnly(const Ray &ray) const;
    Math::Vector3D pos_;
    double radius_;
    double height_;
};

std::ostream &operator<<(std::ostream &out, const Cylinder &ray);

}  // namespace RayTracer



#endif
