/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Vector
*/

#include "Raytracer/math/Vector.hpp"

#include <cmath>
#include <iostream>

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

Vector3D Vector3D::cross(const Vector3D &other) const {
    return Vector3D(this->y * other.z - this->z * other.y,
                    this->z * other.x - this->x * other.z,
                    this->x * other.y - this->y * other.x);
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

bool Vector3D::operator==(const Vector3D other) {
    return this->x == other.x && this->y == other.y && this->z == other.z;
}

bool Vector3D::operator<(const Vector3D &other) const {
    return this->length() < other.length();
}

bool Vector3D::near_zero() const {
    double s = 1e-8;
    return (std::fabs(this->x) < s) && (std::fabs(this->y) < s) &&
           (std::fabs(this->z) < s);
}

void Vector3D::setRange(double range, double newRange) {
    this->x = (this->x * newRange) / range;
    this->y = (this->y * newRange) / range;
    this->z = (this->z * newRange) / range;
}

double Vector3D::getAxis(int axis) const {
    if (axis == 0) return this->x;
    if (axis == 1) return this->y;
    return this->z;
}

void Vector3D::clamp(double min, double max) {
    this->x = std::max(min, std::min(this->x, max));
    this->y = std::max(min, std::min(this->y, max));
    this->z = std::max(min, std::min(this->z, max));
}

Vector3D &Vector3D::clamped(double min, double max) {
    this->clamp(min, max);
    return *this;
}

std::ostream &operator<<(std::ostream &out, const Math::Vector3D &vec) {
    out << "Vector3D(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return out;
}

bool lookUpVector(const libconfig::Setting &setting, Math::Vector3D &vector) {
    bool res = true;

    res &= setting.lookupValue("x", vector.x);
    res &= setting.lookupValue("y", vector.y);
    res &= setting.lookupValue("z", vector.z);
    return res;
}

void writeUpVector(libconfig::Setting &setting, const Math::Vector3D &vector) {
  setting.add("x", libconfig::Setting::TypeFloat) = vector.x;
  setting.add("y", libconfig::Setting::TypeFloat) = vector.y;
  setting.add("z", libconfig::Setting::TypeFloat) = vector.z;
}
}  // namespace Math
