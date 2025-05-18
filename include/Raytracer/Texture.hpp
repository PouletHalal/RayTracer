/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Texture
*/

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <vector>

#include "Raytracer/math/Vector.hpp"
namespace RayTracer {

class Texture {
   public:
    Texture();
    Texture(std::string filePath);
    Math::Vector3D getColor(double u, double v) const;
    bool hasValue(void) const;
    const std::string &getName() const;

    struct TextureInfos {
        size_t width;
        size_t height;
        size_t maxValue;
    };

   private:
    bool haveOne_ = false;
    std::string name;
    std::vector<std::vector<Math::Vector3D>> image_;
};

};  // namespace RayTracer

#endif
