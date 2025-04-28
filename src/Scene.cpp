/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Scene
*/

#include "Scene.hpp"

#include "Utils.hpp"

namespace RayTracer {

HitRecord RayTracer::Scene::hit(const Ray &ray) const {
    HitRecord closest;
    float closestDistance = INFINITY;

    for (const std::shared_ptr<RayTracer::IShape> &shape : this->shapeList) {
        const HitRecord hit = shape->hit(ray);
        if (!hit.missed && hit.t < closestDistance) {
            closestDistance = hit.t;
            closest = hit;
        }
    }
    return closest;
}

void Scene::addShape(const std::shared_ptr<IShape> &shape) {
    this->shapeList.push_front(shape);
}

}  // namespace RayTracer
