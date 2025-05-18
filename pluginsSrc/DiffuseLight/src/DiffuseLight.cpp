/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Lambertarian
*/

#include "../include/DiffuseLight.hpp"

#include <iostream>
#include <libconfig.h++>

#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

DiffuseLight::DiffuseLight(const libconfig::Setting &config) {
    if (!config.lookupValue("red", this->albedo.x)) {
        throw ParsingException(
            "error parsing diffuseLight material, missing \"red\" field");
    }
    if (!config.lookupValue("green", this->albedo.y)) {
        throw ParsingException(
            "error parsing diffuseLight material, missing \"green\" field");
    }
    if (!config.lookupValue("blue", this->albedo.z)) {
        throw ParsingException(
            "error parsing diffuseLight material, missing \"blue\" field");
    }
    this->albedo.setRange(255, 1);
}

void DiffuseLight::save(libconfig::Setting &parent) const {
    libconfig::Setting &mat = parent.add("material", libconfig::Setting::TypeGroup);
    mat.add("type", libconfig::Setting::TypeString) = "diffuseLight";
    libconfig::Setting &data = mat.add("data", libconfig::Setting::TypeGroup);

    data.add("red", libconfig::Setting::TypeFloat) = this->albedo.x * 255;
    data.add("green", libconfig::Setting::TypeFloat) = this->albedo.y * 255;
    data.add("blue", libconfig::Setting::TypeFloat) = this->albedo.z * 255;
}

bool DiffuseLight::scatter(const Ray &, const HitRecord &rec,
                         Math::Vector3D &attenuation, Ray &scattered) const {
    Math::Vector3D scatter_direction =
        rec.normal + Math::Vector3D::random_unit_vector();

    if (scatter_direction.near_zero()) scatter_direction = rec.normal;
    scattered = Ray(rec.p, scatter_direction);
    attenuation = this->albedo;
    return true;
}

void DiffuseLight::setColor(const Math::Vector3D &newColor) {
    this->albedo = newColor;
    this->albedo.setRange(255, 1);
}

Math::Vector3D DiffuseLight::emitted() const {
    return this->albedo;
}

std::unique_ptr<Material> DiffuseLight::duplicate(void) {
    return std::make_unique<DiffuseLight>(*this);
}

extern "C" {
void *entry_point(const libconfig::Setting &config) {
    return new DiffuseLight(config);
}
}
}  // namespace RayTracer
