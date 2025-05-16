/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Lambertarian
*/

#include <libconfig.h++>

#include "plugins/Material.hpp"

namespace RayTracer {

class Lambertian : public Material {
   public:
    Lambertian(const libconfig::Setting &);

    bool scatter(const Ray &r_in, const HitRecord &rec,
                 Math::Vector3D &attenuation, Ray &scattered) const override;
    std::unique_ptr<Material> duplicate(void) override;
    void setColor(const Math::Vector3D &newColor) override;


   private:
    Math::Vector3D albedo;
};

};  // namespace RayTracer
