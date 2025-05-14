/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Shape
*/

#ifndef ASHAPE_HPP
#define ASHAPE_HPP

#include "FixCrossInclude.hpp"
#include "AABB.hpp"
#include "IShape.hpp"

namespace RayTracer {

class AShape : public IShape {
   protected:
    AABB bbox;

   public:
    AABB boundingBox() const override;
};

}  // namespace RayTracer

#endif
