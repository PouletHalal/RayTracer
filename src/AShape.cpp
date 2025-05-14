/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** AShape
*/

#include "AShape.hpp"

namespace RayTracer {

AABB RayTracer::AShape::boundingBox() const {
    return this->bbox;
}

}  // namespace RayTracer
