/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Scene
*/

#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>

#include "HitRecord.hpp"

// TODO : a Scene class

namespace RayTracer {

class Scene {
   public:
    std::list<HitRecord> sendRay() { return std::list<HitRecord>(); };
};

}  // namespace Raytracer

#endif
