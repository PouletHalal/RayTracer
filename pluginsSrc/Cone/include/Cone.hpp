/*
** EPITECH PROJECT, 2025
** disc
** File description:
** disc
*/

#ifndef CONE_HPP
#define CONE_HPP

#include <libconfig.h++>
#include <algorithm>

#include "Interval.hpp"
#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/AShape.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer
{
    class Cone : public AShape
    {
    public:
        double angle;
        double h_;
        Math::Vector3D tipPosition_;
        Math::Vector3D axis_;
        double radius;

        Cone() {};
        Cone(double halfAngle, double height, Math::Vector3D tipPosition, Math::Vector3D axis) : angle(halfAngle), h_(height), tipPosition_(tipPosition), axis_(axis) {};
        Cone(const libconfig::Setting &settings);
        ~Cone() = default;
        void scale(size_t) override {};
        void move(const Math::Vector3D &) override {};
        void rotate(const Math::Vector3D &) override {};
        void setPosition(const Math::Vector3D &) override {};
        HitRecord hits(const Ray &ray, Interval) const override;
        HitRecord hitsCapOnly(const Ray &ray, Interval ray_t) const;
        Math::Vector3D getPointColor(const Math::Vector3D &) const override;
        void save(libconfig::Setting &) const override;
    };
}
#endif
