/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Utils
*/

#ifndef UTILS_HPP
#define UTILS_HPP

#include <limits>
#include <random>

#define PI 3.1415926535897932385
#define EPSILON 1e-14

const double DOUBLE_INFINITY = std::numeric_limits<double>::infinity();

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;

    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
    return int(random_double(min, max+1));
}

#endif
