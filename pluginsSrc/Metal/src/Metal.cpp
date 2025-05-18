/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Lambertarian
*/

#include "../include/Metal.hpp"

#include <libconfig.h++>
#include <memory>

#include "RaytracerCore.hpp"
#include "plugins/Material.hpp"

namespace RayTracer {

Metal::Metal(const libconfig::Setting &config) {
    if (!config.lookupValue("red", this->albedo.x)) {
        throw ParsingException(
            "error parsing lambertian material, missing \"red\" field");
    }
    if (!config.lookupValue("green", this->albedo.y)) {
        throw ParsingException(
            "error parsing lambertian material, missing \"green\" field");
    }
    if (!config.lookupValue("blue", this->albedo.z)) {
        throw ParsingException(
            "error parsing lambertian material, missing \"blue\" field");
    }
    if (!config.lookupValue(("fuzz"), this->fuzz)) {
        throw ParsingException(
            "error parsing lambertian material, missing \"fuzz\" field");
    }
    this->albedo.setRange(255, 1);
}

void Metal::save(libconfig::Setting &parent) const {
    libconfig::Setting &mat = parent.add("material", libconfig::Setting::TypeGroup);
    mat.add("type", libconfig::Setting::TypeString) = "metal";
    libconfig::Setting &data = mat.add("data", libconfig::Setting::TypeGroup);

    data.add("red", libconfig::Setting::TypeFloat) = this->albedo.x * 255;
    data.add("green", libconfig::Setting::TypeFloat) = this->albedo.y * 255;
    data.add("blue", libconfig::Setting::TypeFloat) = this->albedo.z * 255;
    data.add("fuzz", libconfig::Setting::TypeFloat) = this->fuzz;
}

static Math::Vector3D reflect(const Math::Vector3D &v,
                              const Math::Vector3D &n) {
    return v - n * (2 * v.dot(n));
}

bool Metal::scatter(const Ray &r_in, const HitRecord &rec,
                    Math::Vector3D &attenuation, Ray &scattered) const {
    Math::Vector3D reflected = reflect(r_in.dir, rec.normal);

    reflected =
        reflected.normalized() + (Math::Vector3D::random_unit_vector() * fuzz);
    scattered = Ray(rec.p, reflected);
    attenuation = albedo;
    return scattered.dir.dot(rec.normal) > 0;
}

std::unique_ptr<Material> Metal::duplicate(void) {
    return std::make_unique<Metal>(*this);
}

void Metal::setColor(const Math::Vector3D &newColor) {
    this->albedo = newColor;
    this->albedo.setRange(255, 1);
}

extern "C" {
void *entry_point(const libconfig::Setting &config) {
    return new Metal(config);
}
}
}  // namespace RayTracer
