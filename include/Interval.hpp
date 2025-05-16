/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Interval
*/

#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "Utils.hpp"
#include <iostream>

class Interval {
   public:
    double min, max;

    Interval() : min(+DOUBLE_INFINITY), max(-DOUBLE_INFINITY) {
    }

    Interval(double min, double max) : min(min), max(max) {
    }

    Interval(const Interval &a, const Interval &b) {
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

    double size() const {
        return max - min;
    }

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double mid() const {
        return (max + min) / 2;
    }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    Interval expand(double delta) const {
        auto padding = delta / 2;
        return Interval(min - padding, max + padding);
    }

    Interval operator+(const double displacement) const {
        return Interval(this->min + displacement, this->max + displacement);
    }

    Interval &operator+=(const double &displacement) {
        this->min += displacement;
        this->max += displacement;
        return *this;
    }

    
};

std::ostream &operator<<(std::ostream &out, const Interval &ival);

#endif
