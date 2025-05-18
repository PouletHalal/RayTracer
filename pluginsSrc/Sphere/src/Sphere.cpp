/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Sphere
*/

#include "../include/Sphere.hpp"

#include <cmath>
#include <libconfig.h++>
#include <ostream>

#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

Sphere::Sphere() : radius(0) {
    Math::Vector3D rvec = Math::Vector3D(radius, radius, radius);
    this->bbox = AABB(this->pos - (rvec), this->pos + (rvec));
};
Sphere::Sphere(Math::Vector3D pos, double radius) : pos(pos), radius(radius) {
    Math::Vector3D rvec = Math::Vector3D(radius, radius, radius);
    this->bbox = AABB(this->pos - (rvec), this->pos + (rvec));
};

Sphere::Sphere(const libconfig::Setting &settings) {
    try {
        libconfig::Setting &pos = settings.lookup("pos");
        if (!Math::lookUpVector(pos, this->pos)) {
            throw ParsingException(
                "error parsing sphere object, wrong \"pos\" field");
        }
        if (!settings.lookupValue("radius", this->radius)) {
            throw ParsingException(
                "error parsing sphere object, wrong \"radius\" field");
        }
        std::string texture;
        if (settings.lookupValue("texture", texture)) {
            this->texture_ = texture;
        }
        Math::Vector3D rvec = Math::Vector3D(radius, radius, radius);
        this->bbox = AABB(this->pos - (rvec), this->pos + (rvec));
    } catch (const ParsingException &e) {
        throw e;
    } catch (const libconfig::SettingNotFoundException &e) {
        throw ParsingException(e.what());
    }
}

HitRecord Sphere::hits(const Ray &ray, Interval interval) const {
    Math::Vector3D oc = this->pos - ray.pos;
    double a = ray.dir.lengthSquared();
    double h = ray.dir.dot(oc);
    double c = oc.lengthSquared() - radius * radius;

    double discriminant = h * h - a * c;
    if (discriminant < 0) return HitRecord();

    double sqrtd = std::sqrt(discriminant);

    double root = (h - sqrtd) / a;
    if (!interval.contains(root)) {
        root = (h + sqrtd) / a;
        if (!interval.contains(root)) return HitRecord();
    }

    return HitRecord(root, ray, *this, (ray.at(root) - this->pos) / radius,
                     this->material_);
}

Math::Vector3D Sphere::getPointColor(const Math::Vector3D &point) const {
    Math::Vector3D d = this->pos - point;
    d = d / this->radius;
    double u = 0.5 + std::atan2(d.z, d.x) / (2 * M_PI);
    double v = 0.5 + std::asin(d.y) / M_PI;

    return this->texture_.getColor(u, v);
};


void Sphere::move(const Math::Vector3D &offset) {
    this->pos += offset;
}

void Sphere::rotate(const Math::Vector3D &angles) {
    (void)(angles);
}

void Sphere::scale(size_t scale) {
    this->radius *= (double)scale;
}

void Sphere::setPosition(const Math::Vector3D &newPos) {
    this->pos = newPos;
}

std::ostream &operator<<(std::ostream &out, const Sphere &sphere) {
    return out << "Sphere(pos=" << sphere.pos << ", radius=" << sphere.radius
               << ")" << std::endl;
}

void Sphere::save(libconfig::Setting &parent) const {
  libconfig::Setting &sphereSettings =
      parent.add(libconfig::Setting::TypeGroup);
  sphereSettings.add("type", libconfig::Setting::TypeString) = "shape";
  sphereSettings.add("name", libconfig::Setting::TypeString) = "sphere";
  libconfig::Setting &data =
      sphereSettings.add("data", libconfig::Setting::TypeGroup);
  libconfig::Setting &posSettings =
      data.add("pos", libconfig::Setting::TypeGroup);
  Math::writeUpVector(posSettings, this->pos);
  data.add("radius", libconfig::Setting::TypeFloat) = radius;
  if (this->texture_.hasValue()) {
      data.add("texture", libconfig::Setting::TypeString) = this->texture_.getName();
  }
  this->material_->save(sphereSettings);
}

}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
    return new RayTracer::Sphere(config);
}
}
