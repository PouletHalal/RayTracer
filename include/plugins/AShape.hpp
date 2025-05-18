/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** AObject
*/

#ifndef AOBJECT_HPP
#define AOBJECT_HPP

#include <memory>

#include "plugins/Material.hpp"
#include "Raytracer/Texture.hpp"
#include "plugins/IShape.hpp"
#include "AABB.hpp"

namespace RayTracer {

class AABB;
class BVHNode;

class AShape : public IShape {
   public:
    AShape() = default;
    virtual void setMaterial(std::unique_ptr<Material> &) override;
    const AABB &boundingBox() const override;
    void setBoundingBox(const AABB &bbox) override;
    virtual std::unique_ptr<Material> &getMaterial() override;
    bool haveTexture() const;
    BVHNode *getBVH() const override;
    void setBVH(BVHNode *bvhNode) override;
    virtual void save(libconfig::Setting &) const override {return;};

   protected:
    AShape(AABB);
    AABB bbox;
    std::unique_ptr<Material> material_;
    Texture texture_;
    BVHNode *bvhNode;
};

};  // namespace RayTracer

#endif
