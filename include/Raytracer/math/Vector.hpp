/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Vector
*/

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <libconfig.h++>

#include "Utils.hpp"

namespace Math {

class Vector3D {
   public:
    double x;
    double y;
    double z;
    std::vector<std::reference_wrapper<double>>arr = {x, y, z};

    Vector3D() : x(0), y(0), z(0) {};
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {};
    double length() const;
    double lengthSquared() const;
    double dot(const Vector3D &other) const;
    Vector3D cross(const Vector3D &other) const;
    void normalize();
    Vector3D normalized() const;
    void rotateX(double angle);
    Vector3D rotatedX(double angle) const;
    void rotateY(double angle);
    Vector3D rotatedY(double angle) const;
    void rotateZ(double angle);
    Vector3D rotatedZ(double angle) const;
    Vector3D operator-() const;
    Vector3D operator+(const Vector3D &other) const;
    Vector3D operator-(const Vector3D &other) const;
    Vector3D operator*(const Vector3D &other) const;
    Vector3D operator/(const Vector3D &other) const;
    Vector3D &operator+=(const Vector3D &other);
    Vector3D &operator-=(const Vector3D &other);
    Vector3D &operator*=(const Vector3D &other);
    Vector3D &operator/=(const Vector3D &other);
    Vector3D operator+(const double &other) const;
    Vector3D operator-(const double &other) const;
    Vector3D operator*(const double &other) const;
    Vector3D operator/(const double &other) const;
    Vector3D &operator+=(const double &other);
    Vector3D &operator-=(const double &other);
    Vector3D &operator*=(const double &other);
    Vector3D &operator/=(const double &other);
    bool operator==(const Vector3D other);
    bool operator<(const Vector3D &other) const;
    bool near_zero() const;
    void setRange(double range, double newRange);
    double getAxis(int axis) const;
    void clamp(double min, double max);
    Vector3D &clamped(double min, double max);

    static inline Vector3D random() {
        return Vector3D(random_double(), random_double(), random_double());
    }

    static inline Vector3D random(double min, double max) {
        return Vector3D(random_double(min, max), random_double(min, max),
                        random_double(min, max));
    }

    static inline Vector3D random_unit_vector() {
        while (true) {
            Vector3D p = Vector3D::random(-1, 1);
            double lensq = p.lengthSquared();

            if (1e-160 < lensq &&
                lensq <= 1)
                return p / sqrt(lensq);
        }
    }

    static inline Vector3D random_on_hemisphere(const Vector3D &normal) {
        Vector3D on_unit_sphere = Vector3D::random_unit_vector();

        if (on_unit_sphere.dot(normal) > 0.0)
            return on_unit_sphere;
        else
            return -on_unit_sphere;
    }
};

bool lookUpVector(const libconfig::Setting &setting, Vector3D &vector);
void writeUpVector(libconfig::Setting &setting, const Math::Vector3D &vector);

std::ostream &operator<<(std::ostream &out, const Vector3D &vec);

}  // namespace Math

#endif
