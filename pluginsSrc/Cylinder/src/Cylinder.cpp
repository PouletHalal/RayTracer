/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Cylinder
*/

#include "Cylinder.hpp"

#include <cmath>
#include <libconfig.h++>

#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "Utils.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

Cylinder::Cylinder(const libconfig::Setting &settings) {
  try {
    libconfig::Setting &pos = settings.lookup("pos");
    if (!Math::lookUpVector(pos, this->pos_)) {
      throw ParsingException(
          "error parsing cylinder object, wrong \"pos\" field");
    }
    if (!settings.lookupValue("radius", this->radius_) ||
        !settings.lookupValue("height", this->height_)) {
      throw ParsingException(
          "error parsing cylinder object, wrong \"radius / height\" "
          "field");
    }
    std::string texture;
    if (settings.lookupValue("texture", texture)) {
      this->texture_ = texture;
    }
    this->bbox =
        AABB({this->pos_.x - radius_, this->pos_.y, this->pos_.z + radius_},
             {this->pos_.x + this->radius_, this->pos_.y + this->height_,
              this->pos_.z - this->radius_});
  } catch (const ParsingException &e) {
    throw e;
  } catch (const libconfig::SettingNotFoundException &e) {
    throw ParsingException(e.what());
  }
}

HitRecord Cylinder::hits(const Ray &ray, Interval ray_t) const {
  Math::Vector3D oc = ray.pos - pos_;
  double a = ray.dir.x * ray.dir.x + ray.dir.z * ray.dir.z;
  double b = 2.0 * (oc.x * ray.dir.x + oc.z * ray.dir.z);
  double c = oc.x * oc.x + oc.z * oc.z - this->radius_ * this->radius_;

  if (fabs(a) < EPSILON) return hitsCapOnly(ray, ray_t);
  double discriminant = b * b - 4 * a * c;

  if (discriminant < 0) return hitsCapOnly(ray, ray_t);
  double sqrtd = sqrt(discriminant);
  double t1 = (-b - sqrtd) / (2 * a);
  double t2 = (-b + sqrtd) / (2 * a);
  double t = t1;
  double y1 = oc.y + t1 * ray.dir.y;

  if (t1 < EPSILON || y1 < 0 || y1 > height_) {
    t = t2;
    double y2 = oc.y + t2 * ray.dir.y;

    if (t2 < EPSILON || y2 < 0 || y2 > height_) return hitsCapOnly(ray, ray_t);
  }
  Math::Vector3D p = ray.pos + ray.dir * t;
  Math::Vector3D normal(p.x - pos_.x, 0, p.z - pos_.z);
  normal = normal.normalized();
  HitRecord capRecord = hitsCapOnly(ray, ray_t);

  if (capRecord.t > EPSILON && (capRecord.t < t || t < EPSILON))
    return capRecord;

  return HitRecord(t, ray, *this, normal, this->material_);
}

HitRecord Cylinder::hitsCapOnly(const Ray &ray, Interval) const {
  double t_min = INFINITY;
  Math::Vector3D normal;

  if (fabs(ray.dir.y) > EPSILON) {
    double t = (pos_.y - ray.pos.y) / ray.dir.y;
    if (t > EPSILON) {
      Math::Vector3D p = ray.pos + ray.dir * t;
      double distance = (p - Math::Vector3D(pos_.x, pos_.y, pos_.z)).length();
      if (distance <= radius_) {
        t_min = t;
        normal = Math::Vector3D(0, -1, 0);
      }
    }
  }
  if (fabs(ray.dir.y) > EPSILON) {
    double t = (pos_.y + height_ - ray.pos.y) / ray.dir.y;

    if (t > EPSILON && t < t_min) {
      Math::Vector3D p = ray.pos + ray.dir * t;
      double distance =
          (p - Math::Vector3D(pos_.x, pos_.y + height_, pos_.z)).length();

      if (distance <= radius_) {
        t_min = t;
        normal = Math::Vector3D(0, 1, 0);
      }
    }
  }

  if (t_min != INFINITY)
    return HitRecord(t_min, ray, *this, normal, this->material_);

  return HitRecord();
}

void Cylinder::move(const Math::Vector3D &offset) { this->pos_ += offset; }

void Cylinder::rotate(const Math::Vector3D &angles) { (void)(angles); }

void Cylinder::scale(size_t scale) { this->radius_ *= (double)scale; }

void Cylinder::save(libconfig::Setting &parent) const {
    libconfig::Setting &coneSettings =
        parent.add(libconfig::Setting::TypeGroup);
    coneSettings.add("type", libconfig::Setting::TypeString) = "shape";
    coneSettings.add("name", libconfig::Setting::TypeString) = "cylinder";
    libconfig::Setting &data =
        coneSettings.add("data", libconfig::Setting::TypeGroup);
    libconfig::Setting &posSettings =
        data.add("pos", libconfig::Setting::TypeGroup);
    Math::writeUpVector(posSettings, this->pos_);
    data.add("radius", libconfig::Setting::TypeFloat) = this->radius_;
    data.add("height", libconfig::Setting::TypeFloat) = this->height_;
    if (this->texture_.hasValue()) {
        data.add("texture", libconfig::Setting::TypeString) = this->texture_.getName();
    }
    this->material_->save(coneSettings);
}

void Cylinder::setPosition(const Math::Vector3D &newPos) {
  this->pos_ = newPos;
}

Math::Vector3D Cylinder::getPointColor(const Math::Vector3D &point) const {
  double theta = std::atan2(point.x, point.z);
  double raw_u = theta / (2 * M_PI);
  double u = 1 - (raw_u + 0.5);

  double origin = this->pos_.y + this->height_;
  double v = (point.y - origin) / this->height_;

  return this->texture_.getColor(u, v);
}

}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
  return new RayTracer::Cylinder(config);
}
}
