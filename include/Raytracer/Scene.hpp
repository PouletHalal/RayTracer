/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Scene
*/

#ifndef SCENE_HPP
#define SCENE_HPP

#include <libconfig.h++>
#include <memory>
#include <vector>

#include "plugins/AShape.hpp"
#include "plugins/ILight.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

class BVHNode;

#define MAX_SHAPES_PER_SCENE 2000

class Scene : public AShape {
   public:
    Scene();
    Scene(std::unique_ptr<IShape> shape);
    HitRecord hits(const Ray &ray, Interval ray_t) const override;
    void move(const Math::Vector3D &offset) override;
    void rotate(const Math::Vector3D &angles) override;
    void scale(size_t scale) override;
    void setPosition(const Math::Vector3D &newPos) override;
    Math::Vector3D getPointColor(const Math::Vector3D &) const override { return {1, 1, 1}; };
    void save(libconfig::Setting &) const override;
    void addShape(std::unique_ptr<IShape> shape);
    void addLight(std::unique_ptr<ILight> light);

    std::vector<std::unique_ptr<IShape>> shapes_;
    std::vector<std::unique_ptr<ILight>> lights_;
    std::unique_ptr<BVHNode> bvh;
};

};  // namespace RayTracer

#include "BVHNode.hpp"

#endif
