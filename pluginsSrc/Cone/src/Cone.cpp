/*
** EPITECH PROJECT, 2025
** Disc
** File description:
** Disc
*/

#include "../include/Cone.hpp"

#include <libconfig.h++>

#include "Interval.hpp"
#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {
Cone::Cone(const libconfig::Setting &settings) {
  try {
    libconfig::Setting &tip = settings.lookup("tipPosition");
    if (!Math::lookUpVector(tip, this->tipPosition_)) {
      throw ParsingException(
          "error parsing cone object, missing \"tipPosition\" field");
    }
  } catch (const libconfig::SettingNotFoundException &) {
    throw ParsingException(
        "error parsing cone object, missing \"tipPosition\" field");
  }
  try {
    libconfig::Setting &tip = settings.lookup("axis");
    if (!Math::lookUpVector(tip, this->axis_)) {
      throw ParsingException(
          "error parsing cone object, missing \"axis\" field");
    }
  } catch (const libconfig::SettingNotFoundException &) {
    throw ParsingException("error parsing cone object, missing \"axis\" field");
  }
  if (!settings.lookupValue("angle", this->angle)) {
    throw ParsingException(
        "error parsing cone object, missing \"angle\" field");
  }
  if (!settings.lookupValue("height", this->h_)) {
    throw ParsingException(
        "error parsing cone object, missing \"height\" field");
  }
  if (!settings.lookupValue("radius", this->radius)) {
    throw ParsingException(
        "error parsing cone object, missing \"radius\" field");
  }
  std::string texture;
  if (settings.lookupValue("texture", texture)) {
    this->texture_ = texture;
  }
  this->bbox = AABB({-10000, -10000, -10000}, {10000, 10000, 10000});
}

HitRecord Cone::hits(const Ray &ray, Interval ray_t) const {
  Math::Vector3D oc =
      ray.pos -
      tipPosition_;  // calcule entre la pointe du cone et le debut du rayon,
                     // d'où vient le rayon par rapport au cône.

  double k = (radius / h_) * (radius / h_);  // inclinaison du cone

  double a = (ray.dir.x * ray.dir.x) + (ray.dir.z * ray.dir.z) -
             k * (ray.dir.y * ray.dir.y);
  double b = 2 * (oc.x * ray.dir.x + oc.z * ray.dir.z - k * oc.y * ray.dir.y);
  double c = (oc.x * oc.x) + (oc.z * oc.z) - k * (oc.y * oc.y);

  double discriminant = b * b - 4 * a * c;

  if (discriminant < EPSILON) return hitsCapOnly(ray, ray_t);

  double sqrt_disc = std::sqrt(discriminant);
  double t1 = (-b - sqrt_disc) / (2 * a);
  double t2 = (-b + sqrt_disc) / (2 * a);  // distance rayon croise le cone

  double t = t1;
  if (t1 < EPSILON)  // on cherche celui qui est le plus avance
    t = t2;
  if (t < EPSILON)
    return hitsCapOnly(ray, ray_t);  // si les deux sont rayon sont derriere

  Math::Vector3D p = ray.at(t);     // point touche
  double y = p.y - tipPosition_.y;  // si y est entre le sommet et la base

  if (y < 0 || y > h_) return hitsCapOnly(ray, ray_t);

  Math::Vector3D tipImpact =
      p - tipPosition_;  // vecteur point d'impact et pointe
  double r = std::sqrt(tipImpact.x * tipImpact.x + tipImpact.z * tipImpact.z);
  Math::Vector3D normal(tipImpact.x, r * (radius / h_),
                        tipImpact.z);  // calcule normal
  normal = normal.normalized();

  return HitRecord(t, ray, *this, normal, this->material_);
}
HitRecord Cone::hitsCapOnly(const Ray &ray, Interval) const {
  double t_min = INFINITY;
  Math::Vector3D normal;

  if (fabs(ray.dir.y) > EPSILON) {
    double t = (tipPosition_.y + h_ - ray.pos.y) / ray.dir.y;

    if (t > EPSILON && t < t_min) {
      Math::Vector3D p = ray.pos + ray.dir * t;
      double distance = (p - Math::Vector3D(tipPosition_.x, tipPosition_.y + h_,
                                            tipPosition_.z))
                            .length();

      if (distance <= radius) {
        t_min = t;
        normal = Math::Vector3D(0, 1, 0);
      }
    }
  }

  if (t_min != INFINITY)
    return HitRecord(t_min, ray, *this, normal, this->material_);

  return HitRecord();
}

Math::Vector3D Cone::getPointColor(const Math::Vector3D &point) const {
  double theta = std::atan2(point.x, point.z);
  double raw_u = theta / (2 * M_PI);
  double u = 1 - (raw_u + 0.5);

  double origin = this->tipPosition_.x + this->h_;
  double v = (point.y - origin) / this->h_;

  return this->texture_.getColor(u, v);
}

void Cone::save(libconfig::Setting &parent) const {
  libconfig::Setting &coneSettings = parent.add(libconfig::Setting::TypeGroup);
  coneSettings.add("type", libconfig::Setting::TypeString) = "shape";
  coneSettings.add("name", libconfig::Setting::TypeString) = "cone";
  libconfig::Setting &data =
      coneSettings.add("data", libconfig::Setting::TypeGroup);
  libconfig::Setting &posSettings =
      data.add("tipPosition", libconfig::Setting::TypeGroup);
  Math::writeUpVector(posSettings, this->tipPosition_);
  libconfig::Setting &uSettings =
      data.add("axis", libconfig::Setting::TypeGroup);
  Math::writeUpVector(uSettings, this->axis_);
  data.add("angle", libconfig::Setting::TypeFloat) = this->angle;
  data.add("height", libconfig::Setting::TypeFloat) = this->h_;
  data.add("radius", libconfig::Setting::TypeFloat) = this->radius;
  if (this->texture_.hasValue()) {
    data.add("texture", libconfig::Setting::TypeString) =
        this->texture_.getName();
  }
  this->material_->save(coneSettings);
}

}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
  return new RayTracer::Cone(config);
}
}
