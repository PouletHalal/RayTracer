/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Plane
*/

#include "Plane.hpp"

#include <cmath>
#include <libconfig.h++>
#include <ostream>

#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "Utils.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

Plane::Plane(Math::Vector3D pos, Math::Vector3D orientation) : pos_(pos), orientation_(orientation) {};

Plane::Plane(const libconfig::Setting &settings) {
    try {
        libconfig::Setting &pos = settings.lookup("pos");
        libconfig::Setting &normal = settings.lookup("orientation");
        if (!Math::lookUpVector(pos, this->pos_)) {
            throw ParsingException(
                "error parsing plane object, wrong \"pos\" field");
        }
        if (!Math::lookUpVector(normal, this->orientation_)) {
            throw ParsingException(
                "error parsing plane object, wrong \"orientation\" field");
        }
        std::string texture;
        if (settings.lookupValue("texture", texture)) {
            this->texture_ = texture;
        }
    } catch (const ParsingException &e) {
        throw e;
    } catch (const libconfig::SettingNotFoundException &e) {
        throw ParsingException(e.what());
    }
}

HitRecord Plane::hits(const Ray &ray, Interval ray_t) const {
    double denom = orientation_.dot(ray.dir);
    if (std::abs(denom) > EPSILON) {
        Math::Vector3D p0l0 = pos_ - ray.pos;
        double t = p0l0.dot(orientation_) / denom;
        if (t >= 0 && ray_t.contains(t)) {
            return HitRecord(t, ray, *this, this->orientation_, this->material_);
        }
    }
    return HitRecord();
}

void Plane::move(const Math::Vector3D &offset) { this->pos_ += offset; }

void Plane::rotate(const Math::Vector3D &angles) {
    (void)(angles);
}

void Plane::scale(size_t) { return; }

void Plane::setPosition(const Math::Vector3D &newPos) { this->pos_ = newPos; }

std::ostream &operator<<(std::ostream &os, const Plane &) {return os;}

void Plane::save(libconfig::Setting &parent) const {
    libconfig::Setting &planeSettings =
        parent.add(libconfig::Setting::TypeGroup);
    planeSettings.add("type", libconfig::Setting::TypeString) = "shape";
    planeSettings.add("name", libconfig::Setting::TypeString) = "cylinder";
    libconfig::Setting &data =
        planeSettings.add("data", libconfig::Setting::TypeGroup);
    libconfig::Setting &posSettings =
        data.add("pos", libconfig::Setting::TypeGroup);
    Math::writeUpVector(posSettings, this->pos_);
    libconfig::Setting &orientationSettings =
        data.add("orientation", libconfig::Setting::TypeGroup);
    Math::writeUpVector(orientationSettings, this->orientation_);
    if (this->texture_.hasValue()) {
        data.add("texture", libconfig::Setting::TypeString) = this->texture_.getName();
    }
    this->material_->save(planeSettings);
}

}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
    return new RayTracer::Plane(config);
}
}
