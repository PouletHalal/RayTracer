/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Rectangle
*/

#include "plugins/AShape.hpp"
#include <memory>
#include "AABB.hpp"
#include "plugins/Material.hpp"

namespace RayTracer {

AShape::AShape(AABB box): bbox(std::move(box)) {
}

void AShape::setMaterial(std::unique_ptr<Material> &newMaterial) {
    this->material_.swap(newMaterial);
};

const AABB &AShape::boundingBox() const {
    return this->bbox;
}

void AShape::setBoundingBox(const AABB &bbox) {
    this->bbox.setBoundingBox(bbox);
}

std::unique_ptr<Material> &AShape::getMaterial() {
    return this->material_;
}

bool AShape::haveTexture() const {
    return this->texture_.hasValue();
}

BVHNode *AShape::getBVH() const {
    return this->bvhNode;
}

void AShape::setBVH(BVHNode *bvhNode) {
    this->bvhNode = bvhNode;
}

}  // namespace RayTracer
