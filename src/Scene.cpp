/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Scene
*/

#include "Scene.hpp"

#include "Utils.hpp"

namespace RayTracer {

Scene::Scene(std::shared_ptr<IShape> shape) {
    this->addShape(shape);
}

Scene::Scene(std::vector<std::shared_ptr<IShape>> shapeList) {
    for (std::shared_ptr<IShape> shape : shapeList)
        this->addShape(shape);
}

HitRecord RayTracer::Scene::hit(const Ray &ray, Interval interval) const {
    HitRecord closest;
    float closestDistance = DOUBLE_INFINITY;

    for (const std::shared_ptr<RayTracer::IShape> &shape : this->shapeList) {
        const HitRecord hit = shape->hit(ray, interval);
        if (!hit.missed && hit.t < closestDistance) {
            closestDistance = hit.t;
            closest = hit;
        }
    }
    return closest;
}

void Scene::addShape(const std::shared_ptr<IShape> &shape) {
    this->shapeList.push_back(shape);
    this->bbox = AABB(bbox, shape->boundingBox());
}

}  // namespace RayTracer
