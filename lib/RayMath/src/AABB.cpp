/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** AABB
*/

#include "BVHNode.hpp"

#include <memory>

#include "plugins/Material.hpp"

#define DEBUG_COLOR Math::Vector3D(1, 0, 1)

namespace RayTracer {

LambertianDebug::LambertianDebug(const Math::Vector3D &albedo)
    : albedo(albedo) {
}

bool LambertianDebug::scatter(const Ray &, const HitRecord &rec,
                              Math::Vector3D &attenuation,
                              Ray &scattered) const {
    Math::Vector3D scatter_direction =
        rec.normal + Math::Vector3D::random_unit_vector();

    if (scatter_direction.near_zero()) scatter_direction = rec.normal;
    scattered = Ray(rec.p, scatter_direction);
    attenuation = this->albedo;
    return true;
}

std::unique_ptr<Material> LambertianDebug::duplicate() {
    return std::make_unique<LambertianDebug>(*this);
}

AABB::AABB(): material_(std::make_unique<LambertianDebug>(DEBUG_COLOR)) {
}

AABB::AABB(const AABB &bbox) {
    this->setBoundingBox(bbox);
}

AABB::AABB(const Interval &x, const Interval &y, const Interval &z)
    : x(x), y(y), z(z), material_(std::make_unique<LambertianDebug>(DEBUG_COLOR)) {
    padToMinimums();
}

AABB::AABB(const Math::Vector3D &a, const Math::Vector3D &b): material_(std::make_unique<LambertianDebug>(DEBUG_COLOR)) {
    x = (a.x <= b.x) ? Interval(a.x, b.x) : Interval(b.x, a.x);
    y = (a.y <= b.y) ? Interval(a.y, b.y) : Interval(b.y, a.y);
    z = (a.z <= b.z) ? Interval(a.z, b.z) : Interval(b.z, a.z);

    padToMinimums();
}

AABB::AABB(const AABB &box0, const AABB &box1): material_(std::make_unique<LambertianDebug>(DEBUG_COLOR)) {
    x = Interval(box0.x, box1.x);
    y = Interval(box0.y, box1.y);
    z = Interval(box0.z, box1.z);
}

AABB &AABB::operator=(const RayTracer::AABB &bbox) {
    this->setBoundingBox(bbox);
    return *this;
}

const Interval &AABB::axisInterval(int n) const {
    if (n == 1) return y;
    if (n == 2) return z;
    return x;
}

bool AABB::trueHit(const Ray &r, Interval ray_t) const {
    for (int axis = 0; axis < 3; axis++) {
        const Interval &ax = axisInterval(axis);
        const double adinv = 1.0 / r.dir.arr[axis];

        const double t0 = (ax.min - r.pos.arr[axis]) * adinv;
        const double t1 = (ax.max - r.pos.arr[axis]) * adinv;

        if (t0 < t1) {
            if (t0 > ray_t.min) ray_t.min = t0;
            if (t1 < ray_t.max) ray_t.max = t1;
        } else {
            if (t1 > ray_t.min) ray_t.min = t1;
            if (t0 < ray_t.max) ray_t.max = t0;
        }

        if (ray_t.max <= ray_t.min) return false;
    }
    return true;
}

HitRecord AABB::hits(const Ray &r, Interval ray_t) const {
    Math::Vector3D v_min(this->x.min, this->y.min, this->z.min);
    Math::Vector3D v_max(this->x.max, this->y.max, this->z.max);
    double t_near = ray_t.min;
    double t_far = ray_t.max;
    int entry_axis = -1;
    int exit_axis = -1;
    double entry_sign = 0.0;
    double exit_sign = 0.0;

    bool inside = r.pos.x >= v_min.x && r.pos.x <= v_max.x &&
                  r.pos.y >= v_min.y && r.pos.y <= v_max.y &&
                  r.pos.z >= v_min.z && r.pos.z <= v_max.z;

    for (int axis = 0; axis < 3; axis++) {
        double invD = 1.0 / r.dir.getAxis(axis);
        double t0 = (v_min.getAxis(axis) - r.pos.getAxis(axis)) * invD;
        double t1 = (v_max.getAxis(axis) - r.pos.getAxis(axis)) * invD;

        double sign = invD < 0 ? -1.0 : 1.0;

        if (invD < 0.0) std::swap(t0, t1);

        if (t0 > t_near) {
            t_near = t0;
            entry_axis = axis;
            entry_sign = sign;
        }

        if (t1 < t_far) {
            t_far = t1;
            exit_axis = axis;
            exit_sign = sign;
        }

        if (t_far <= t_near) {
            return HitRecord();
        }
    }

    if (inside) {
        if (!ray_t.surrounds(t_far))
            return HitRecord();
    } else {
        if (!ray_t.surrounds(t_near))
            return HitRecord();
    }

    Math::Vector3D normal;
    if (inside) {
        if (exit_axis == 0) normal.x = exit_sign;
        if (exit_axis == 1) normal.y = exit_sign;
        if (exit_axis == 2) normal.z = exit_sign;
    } else {
        if (entry_axis == 0) normal.x = entry_sign;
        if (entry_axis == 1) normal.y = entry_sign;
        if (entry_axis == 2) normal.z = entry_sign;
    }
    return HitRecord(inside ? t_far : t_near, r, *this, normal, this->material_);
}

int AABB::longestAxis() const {
    if (x.size() > y.size())
        return x.size() > z.size() ? 0 : 2;
    else
        return y.size() > z.size() ? 1 : 2;
}

void AABB::padToMinimums() {
    double delta = 0.0001;
    if (x.size() < delta) x = x.expand(delta);
    if (y.size() < delta) y = y.expand(delta);
    if (z.size() < delta) z = z.expand(delta);
}

void AABB::move(const Math::Vector3D &offset) {
    this->x += offset.x;
    this->y += offset.y;
    this->z += offset.z;
}

void AABB::rotate(const Math::Vector3D &angles) {
    (void)(angles);
}

void AABB::scale(size_t scale) {
    this->x.expand(this->x.size() * (static_cast<double>(scale) - 1));
    this->y.expand(this->y.size() * (static_cast<double>(scale) - 1));
    this->z.expand(this->z.size() * (static_cast<double>(scale) - 1));
}

void AABB::setPosition(const Math::Vector3D &newPos) {
    Interval ival(newPos.x, newPos.x);

    ival.expand(this->x.size());
    this->x = ival;
    ival = Interval(newPos.y, newPos.y);
    ival.expand(this->y.size());
    this->y = ival;
    ival = Interval(newPos.z, newPos.z);
    ival.expand(this->z.size());
    this->z = ival;
}

void AABB::setBoundingBox(const AABB &bbox) {
    this->x = bbox.x;
    this->y = bbox.y;
    this->z = bbox.z;
}

void AABB::setMaterial(std::unique_ptr<Material> &newMaterial) {
    this->material_.swap(newMaterial);
};

void AABB::reset(void){
    this->x.min = DOUBLE_INFINITY;
    this->x.max = -DOUBLE_INFINITY;
    this->y.min = DOUBLE_INFINITY;
    this->y.max = -DOUBLE_INFINITY;
    this->z.min = DOUBLE_INFINITY;
    this->z.max = -DOUBLE_INFINITY;
}

const AABB &AABB::boundingBox() const {
    return *this;
}

std::ostream &operator<<(std::ostream &out, const AABB &aabb) {
    std::cout << aabb.axisInterval(0) << std::endl;
    std::cout << aabb.axisInterval(1) << std::endl;
    std::cout << aabb.axisInterval(2) << std::endl;
    return out;
}

}  // namespace RayTracer
