/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Utils
*/

#include "Utils.hpp"

#include "Vector.hpp"

bool isClose(Math::Vector3D vec1, Math::Vector3D vec2,
             double threshold = 1e-14) {
    return std::abs(vec1.x - vec2.x) < threshold &&
           std::abs(vec1.y - vec2.y) < threshold &&
           std::abs(vec1.z - vec2.z) < threshold;
}
