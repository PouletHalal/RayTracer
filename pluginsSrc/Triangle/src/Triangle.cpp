/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle
*/

#include "../include/Triangle.hpp"

#include <cmath>
#include <libconfig.h++>

#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "AABB.hpp"

namespace RayTracer {

Triangle::Triangle(const libconfig::Setting &settings) {
    try {
        libconfig::Setting &a = settings.lookup("a");
        libconfig::Setting &b = settings.lookup("b");
        libconfig::Setting &c = settings.lookup("c");
        if (!Math::lookUpVector(a, this->a))
            throw ParsingException(
                "error parsing triangle object, wrong \"a\" field");
        if (!Math::lookUpVector(b, this->b))
            throw ParsingException(
                "error parsing triangle object, wrong \"b\" field");
        if (!Math::lookUpVector(c, this->c))
            throw ParsingException(
                "error parsing triangle object, wrong \"c\" field");
        std::string texture;
        this->bbox = AABB(AABB(this->a, this->b), AABB(this->c, this->c));
        if (settings.lookupValue("texture", texture)) {
            this->texture_ = texture;
        }
    } catch (const libconfig::SettingNotFoundException &e) {
        throw ParsingException(e.what());
    } catch (const ParsingException &e) {
        throw ParsingException(e.what());
    }
}

void Triangle::scale(size_t scale) {
    Math::Vector3D center = (a + b + c) / 3.0;

    a = center + (a - center) * (double)scale / 100.;
    b = center + (b - center) * (double)scale / 100.;
    c = center + (c - center) * (double)scale / 100.;
}

void Triangle::setPosition(const Math::Vector3D &) {
}

void Triangle::move(const Math::Vector3D &pos) {
  a += pos;
  b += pos;
  c += pos;
  this->bbox.move(pos);
}

void Triangle::rotate(const Math::Vector3D &angles) {
    // Math::Vector3D center = (a + b + c) / 3;
    // Math::Vector3D toOrigin = -center;
    // a += toOrigin;
    // b += toOrigin;
    // c += toOrigin;
    if (angles.x) {
        a.rotateX(angles.x);
        b.rotateX(angles.x);
        c.rotateX(angles.x);
    }
    if (angles.y) {
        a.rotateY(angles.y);
        b.rotateY(angles.y);
        c.rotateY(angles.y);
    }
    if (angles.z) {
        a.rotateZ(angles.x);
        b.rotateZ(angles.x);
        c.rotateZ(angles.x);
    }
    // a -= toOrigin;
    // b -= toOrigin;
    // c -= toOrigin;
}

void Triangle::save(libconfig::Setting &parent) const {
    libconfig::Setting &sphereSettings =
        parent.add(libconfig::Setting::TypeGroup);
    sphereSettings.add("type", libconfig::Setting::TypeString) = "shape";
    sphereSettings.add("name", libconfig::Setting::TypeString) = "triangle";
    libconfig::Setting &data =
        sphereSettings.add("data", libconfig::Setting::TypeGroup);


    libconfig::Setting &aSettings =
        data.add("a", libconfig::Setting::TypeGroup);
    libconfig::Setting &bSettings =
        data.add("b", libconfig::Setting::TypeGroup);
    libconfig::Setting &cSettings =
        data.add("c", libconfig::Setting::TypeGroup);

    Math::writeUpVector(aSettings, this->a);
    Math::writeUpVector(bSettings, this->b);
    Math::writeUpVector(cSettings, this->c);
    if (this->texture_.hasValue()) {
        data.add("texture", libconfig::Setting::TypeString) = this->texture_.getName();
    }
    this->material_->save(sphereSettings);
}

HitRecord Triangle::hits(const Ray &ray, Interval ray_t) const {
  Math::Vector3D ab = b - a;
  Math::Vector3D ac = c - a;
  Math::Vector3D n = ab.cross(ac);

  double dot = n.dot(ray.dir);
  if (fabs(dot) < EPSILON) return HitRecord();
  double d = -n.dot(a);
  double t = -(n.dot(ray.pos) + d) / dot;

  if (t < 0) return HitRecord();
  Math::Vector3D p = ray.pos + ray.dir * t;
  Math::Vector3D ne;

  Math::Vector3D ap = p - a;
  ne = ab.cross(ap);
  if (n.dot(ne) < 0) return HitRecord();

  Math::Vector3D bc = c - b;
  Math::Vector3D bp = p - b;
  ne = bc.cross(bp);
  if (n.dot(ne) < 0) return HitRecord();

  Math::Vector3D ca = a - c;
  Math::Vector3D cp = p - c;
  ne = ca.cross(cp);
  if (n.dot(ne) < 0) return HitRecord();

  if (!ray_t.contains(t)) return HitRecord();

  return HitRecord(t, ray, *this, n.normalized(), this->material_);
}

Math::Vector3D Triangle::getPointColor(const Math::Vector3D &point) const {
    double zLenght = Math::Vector3D(this->a.x, this->b.y, this->c.z).length();
    double yBase = std::min(this->a.y, std::min(this->b.y, this->c.y));
    double v = (point.z - yBase) / zLenght;

    return this->texture_.getColor(point.length(), v);
}

}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
    return new RayTracer::Triangle(config);
}

void *value_entry_point(Math::Vector3D a, Math::Vector3D b, Math::Vector3D c) {
    return new RayTracer::Triangle(a, b, c);
}
}
