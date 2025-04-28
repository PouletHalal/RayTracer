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
#include "IShape.hpp"

// TODO : a Scene class

namespace RayTracer {

class Scene : public IShape {
private:
    std::list<std::shared_ptr<IShape>> shapeList;

   public:
    HitRecord hit(const Ray &ray) const override;
    void addShape(const std::shared_ptr<IShape> &shape);
};

}  // namespace RayTracer

#endif
