/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Scene
*/

#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "AShape.hpp"
#include "HitRecord.hpp"

// TODO : a Scene class

namespace RayTracer {

class Scene : public AShape {
   public:
   Scene() = default;
   Scene(std::shared_ptr<IShape> shape);
   Scene(std::vector<std::shared_ptr<IShape>> shapeList);
   HitRecord hit(const Ray &ray, Interval interval) const override;
   void addShape(const std::shared_ptr<IShape> &shape);

    std::vector<std::shared_ptr<IShape>> shapeList;
};

}  // namespace RayTracer

#endif
