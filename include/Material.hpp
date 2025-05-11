#ifndef MATERIAL_H
#define MATERIAL_H

#include "IShape.hpp"
#include "Vector.hpp"

namespace RayTracer {

class HitRecord;  // this fix cross include, trust the process

class Material {
   public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &r_in, const HitRecord &rec,
                         Math::Vector3D &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material {
   public:
    Lambertian(const Math::Vector3D &albedo) : albedo(albedo) {
    }

    bool scatter(const Ray &r_in, const HitRecord &rec,
                 Math::Vector3D &attenuation, Ray &scattered) const override;

   private:
    Math::Vector3D albedo;
};

class Metal : public Material {
   public:
    Metal(const Math::Vector3D &albedo, double fuzz)
        : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {
    }

    bool scatter(const Ray &r_in, const HitRecord &rec,
                 Math::Vector3D &attenuation, Ray &scattered) const override;

   private:
    Math::Vector3D albedo;
    double fuzz;
};

class Dielectric : public Material {
   public:
    Dielectric(double refraction_index) : refraction_index(refraction_index) {
    }

    bool scatter(const Ray &r_in, const HitRecord &rec,
                 Math::Vector3D &attenuation, Ray &scattered) const override;

   private:
    double refraction_index;
};

}  // namespace RayTracer

#endif
