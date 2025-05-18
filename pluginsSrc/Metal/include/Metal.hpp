/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Lambertarian
*/

#include <libconfig.h++>

#include "plugins/Material.hpp"

namespace RayTracer {

class Metal : public Material {
   public:
    Metal(const libconfig::Setting &);

    bool scatter(const Ray &r_in, const HitRecord &rec,
                 Math::Vector3D &attenuation, Ray &scattered) const override;
    std::unique_ptr<Material> duplicate(void) override;
    void setColor(const Math::Vector3D &newColor) override;
    void save(libconfig::Setting &parent) const override;

   private:
    Math::Vector3D albedo;
    double fuzz;
};

};  // namespace RayTracer
