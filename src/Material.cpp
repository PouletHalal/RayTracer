/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Material
*/

#include "Material.hpp"

namespace RayTracer {

inline Math::Vector3D reflect(const Math::Vector3D &v,
                              const Math::Vector3D &n) {
    return v - n * (2 * v.dot(n));
}

inline Math::Vector3D refract(const Math::Vector3D &uv, const Math::Vector3D &n,
                              double etai_over_etat) {
    double cos_theta = std::fmin((-uv).dot(n), 1.0);
    Math::Vector3D r_out_perp = (uv + n * cos_theta) * etai_over_etat;
    Math::Vector3D r_out_parallel =
        n * -std::sqrt(std::fabs(1.0 - r_out_perp.lengthSquared()));
    return r_out_perp + r_out_parallel;
}

static double reflectance(double cosine, double refraction_index) {
    // Use Schlick's approximation for reflectance.
    double r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}

bool Lambertian::scatter(const Ray &r_in, const HitRecord &rec,
                         Math::Vector3D &attenuation, Ray &scattered) const {
    // Math::Vector3D albedo = (rec.normal + 1) / 2;
    Math::Vector3D scatter_direction =
        rec.normal + Math::Vector3D::random_unit_vector();

    if (scatter_direction.near_zero()) scatter_direction = rec.normal;
    scattered = Ray(rec.p, scatter_direction);
    attenuation = this->albedo;
    return true;
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

bool Dielectric::scatter(const Ray &r_in, const HitRecord &rec,
                         Math::Vector3D &attenuation, Ray &scattered) const {
    attenuation = Math::Vector3D(1.0, 1.0, 1.0);
    double ri = rec.frontFace ? (1.0 / refraction_index) : refraction_index;

    Math::Vector3D unit_direction = r_in.dir.normalized();
    double cos_theta = std::fmin((-unit_direction).dot(rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    Math::Vector3D direction;

    if (cannot_refract || reflectance(cos_theta, ri) > random_double())
        direction = reflect(unit_direction, rec.normal);
    else
        direction = refract(unit_direction, rec.normal, ri);

    scattered = Ray(rec.p, direction);
    return true;
}

}  // namespace RayTracer
