/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Vector
*/

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>

namespace Math {

class Vector3D {
   public:
    double x;
    double y;
    double z;

    Vector3D() : x(0), y(0), z(0) {};
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {};
    double length() const;
    double dot(const Vector3D &other) const;
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
};

std::ostream &operator<<(std::ostream &out, const Vector3D &vec);

}  // namespace Math

#endif
