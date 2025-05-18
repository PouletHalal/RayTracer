/*
** EPITECH PROJECT, 2025
** Quad
** File description:
** Quad
*/

#include "../include/Quad.hpp"

#include <algorithm>
#include <libconfig.h++>

#include "AABB.hpp"
#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"

namespace RayTracer {
Quad::Quad(const libconfig::Setting &settings) {
    try {
        libconfig::Setting &tip = settings.lookup("origin");
        if (!Math::lookUpVector(tip, this->_origin)) {
            throw ParsingException(
                "error parsing quad object, missing \"origin\" field");
        }
    } catch (const libconfig::SettingNotFoundException &) {
        throw ParsingException(
            "error parsing quad object, missing \"origin\" field");
    }
    try {
        libconfig::Setting &tip = settings.lookup("u");
        if (!Math::lookUpVector(tip, this->_u)) {
            throw ParsingException(
                "error parsing quad object, missing \"u\" field");
        }
    } catch (const libconfig::SettingNotFoundException &) {
        throw ParsingException(
            "error parsing quad object, missing \"u\" field");
    }
    try {
        libconfig::Setting &tip = settings.lookup("v");
        if (!Math::lookUpVector(tip, this->_v)) {
            throw ParsingException(
                "error parsing quad object, missing \"v\" field");
        }
    } catch (const libconfig::SettingNotFoundException &) {
        throw ParsingException(
            "error parsing quad object, missing \"v\" field");
    }

    _normal = _u.cross(_v).normalized();

    Math::Vector3D origin = _origin;
    Math::Vector3D p1 = _origin + _u;
    Math::Vector3D p2 = _origin + _v;
    Math::Vector3D p3 = _origin + _u + _v;

    Math::Vector3D min = {std::min({origin.x, p1.x, p2.x, p3.x}),
                          std::min({origin.y, p1.y, p2.y, p3.y}),
                          std::min({origin.z, p1.z, p2.z, p3.z})};
    Math::Vector3D max = {std::max({origin.x, p1.x, p2.x, p3.x}),
                          std::max({origin.y, p1.y, p2.y, p3.y}),
                          std::max({origin.z, p1.z, p2.z, p3.z})};

    _box = Box3D(min, max);
    this->bbox = AABB(min * 100, max * 100);
}

Quad::Quad(Math::Vector3D q, Math::Vector3D u, Math::Vector3D v)
    : _origin(q), _u(u), _v(v) {
    _normal = u.cross(v).normalized();

    Math::Vector3D origin = _origin;
    Math::Vector3D p1 = _origin + _u;
    Math::Vector3D p2 = _origin + _v;
    Math::Vector3D p3 = _origin + _u + _v;

    double min_x = std::min({origin.x, p1.x, p2.x, p3.x});
    double min_y = std::min({origin.y, p1.y, p2.y, p3.y});
    double min_z = std::min({origin.z, p1.z, p2.z, p3.z});
    double max_x = std::max({origin.x, p1.x, p2.x, p3.x});
    double max_y = std::max({origin.y, p1.y, p2.y, p3.y});
    double max_z = std::max({origin.z, p1.z, p2.z, p3.z});
    _box = Box3D({min_x, min_y, min_z}, {max_x, max_y, max_z});
}

HitRecord Quad::hits(const Ray &ray, Interval) const {
    double denom = _normal.dot(ray.dir);

    if (std::abs(denom) < 1e-6) return HitRecord();

    double t = (_origin - ray.pos).dot(_normal) / denom;

    if (t < 0.001) return HitRecord();

    Math::Vector3D p = ray.at(t);

    double u_coord = (p - _origin).dot(_u) / _u.dot(_u);
    double v_coord = (p - _origin).dot(_v) / _v.dot(_v);

    if (u_coord < 0 || u_coord > 1 || v_coord < 0 || v_coord > 1)
        return HitRecord();
    return HitRecord(t, ray, *this, _normal, this->material_);
}

void Quad::move(const Math::Vector3D &) {};
void Quad::rotate(const Math::Vector3D &) {};
void Quad::scale(size_t) {};
void Quad::setPosition(const Math::Vector3D &) {};

void Quad::setMaterial(std::unique_ptr<Material> &m) {
    this->material_ = m->duplicate();
    this->_box.setMaterial(m);
}

void Quad::save(libconfig::Setting &parent) const {
  libconfig::Setting &quadSettings =
      parent.add(libconfig::Setting::TypeGroup);
  quadSettings.add("type", libconfig::Setting::TypeString) = "shape";
  quadSettings.add("name", libconfig::Setting::TypeString) = "quad";
  libconfig::Setting &data =
      quadSettings.add("data", libconfig::Setting::TypeGroup);
  libconfig::Setting &posSettings =
      data.add("origin", libconfig::Setting::TypeGroup);
  Math::writeUpVector(posSettings, this->_origin);
  libconfig::Setting &uSettings =
      data.add("u", libconfig::Setting::TypeGroup);
  Math::writeUpVector(uSettings, this->_origin);
  libconfig::Setting &vSettings =
      data.add("v", libconfig::Setting::TypeGroup);
  Math::writeUpVector(vSettings, this->_origin);
  this->material_->save(quadSettings);
}

}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
    return new RayTracer::Quad(config);
}
}
