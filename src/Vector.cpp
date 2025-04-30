/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Vector
*/

#include "Vector.hpp"

#include <cmath>

namespace Math {

double Vector3D::length() const {
    return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

double Vector3D::lengthSquared() const {
    return this->x * this->x + this->y * this->y + this->z * this->z;
}

double Vector3D::dot(const Vector3D &other) const {
    return this->x * other.x + this->y * other.y + this->z * other.z;
}

void Vector3D::normalize() {
    *this = this->normalized();
}

Vector3D Vector3D::normalized() const {
    return *this / this->length();
}

void Vector3D::rotateX(double angle) {
    *this = this->rotatedX(angle);
}

Vector3D Vector3D::rotatedX(double angle) const {
    return Vector3D(this->x,
                    this->y * std::cos(angle) + this->z * -std::sin(angle),
                    this->y * std::sin(angle) + this->z * std::cos(angle));
}

void Vector3D::rotateY(double angle) {
    *this = this->rotatedY(angle);
}

Vector3D Vector3D::rotatedY(double angle) const {
    return Vector3D(this->x * std::cos(angle) + this->z * std::sin(angle),
                    this->y,
                    this->x * -std::sin(angle) + this->z * std::cos(angle));
}

void Vector3D::rotateZ(double angle) {
    *this = this->rotatedZ(angle);
}

Vector3D Vector3D::rotatedZ(double angle) const {
    return Vector3D(this->x * std::cos(angle) + this->y * -std::sin(angle),
                    this->x * std::sin(angle) + this->y * std::cos(angle),
                    this->z);
}

Vector3D Vector3D::operator-() const {
    return Vector3D(-this->x, -this->y, -this->z);
}

Vector3D Vector3D::operator+(const Vector3D &other) const {
    return Vector3D(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vector3D Vector3D::operator-(const Vector3D &other) const {
    return Vector3D(this->x - other.x, this->y - other.y, this->z - other.z);
}

Vector3D Vector3D::operator*(const Vector3D &other) const {
    return Vector3D(this->x * other.x, this->y * other.y, this->z * other.z);
}

Vector3D Vector3D::operator/(const Vector3D &other) const {
    return Vector3D(this->x / other.x, this->y / other.y, this->z / other.z);
}

Vector3D &Vector3D::operator+=(const Vector3D &other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}

Vector3D &Vector3D::operator-=(const Vector3D &other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}

Vector3D &Vector3D::operator*=(const Vector3D &other) {
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
    return *this;
}

Vector3D &Vector3D::operator/=(const Vector3D &other) {
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
    return *this;
}

Vector3D Vector3D::operator+(const double &other) const {
    return Vector3D(this->x + other, this->y + other, this->z + other);
}

Vector3D Vector3D::operator-(const double &other) const {
    return Vector3D(this->x - other, this->y - other, this->z - other);
}

Vector3D Vector3D::operator*(const double &other) const {
    return Vector3D(this->x * other, this->y * other, this->z * other);
}

Vector3D Vector3D::operator/(const double &other) const {
    return Vector3D(this->x / other, this->y / other, this->z / other);
}

Vector3D &Vector3D::operator+=(const double &other) {
    this->x += other;
    this->y += other;
    this->z += other;
    return *this;
}

Vector3D &Vector3D::operator-=(const double &other) {
    this->x -= other;
    this->y -= other;
    this->z -= other;
    return *this;
}

Vector3D &Vector3D::operator*=(const double &other) {
    this->x *= other;
    this->y *= other;
    this->z *= other;
    return *this;
}

Vector3D &Vector3D::operator/=(const double &other) {
    this->x /= other;
    this->y /= other;
    this->z /= other;
    return *this;
}

std::ostream &operator<<(std::ostream &out, const Vector3D &vec) {
    out << "Vector3D(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return out;
}

}  // namespace Math
