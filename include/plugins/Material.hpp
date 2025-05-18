/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** AObject
*/

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <memory>

#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/IPlugin.hpp"

namespace RayTracer {

class HitRecord;  // this fix cross include, trust the process

class Material : public IPlugin {
   public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &r_in, const HitRecord &rec,
                         Math::Vector3D &attenuation, Ray &scattered) const = 0;
    virtual std::unique_ptr<Material> duplicate(void) = 0;
    virtual void setColor(const Math::Vector3D &newColor) = 0;
    virtual void save(libconfig::Setting &parent) const = 0;
    virtual Math::Vector3D emitted() const {
        return Math::Vector3D(0, 0, 0);
    }
};

};  // namespace RayTracer

Math::Vector3D reflect(const Math::Vector3D &v, const Math::Vector3D &n);
Math::Vector3D refract(const Math::Vector3D &uv, const Math::Vector3D &n,
                       double etai_over_etat);
double reflectance(double cosine, double refraction_index);

#endif
