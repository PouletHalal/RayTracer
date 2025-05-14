/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** AABB
*/

#include "AABB.hpp"

#define DEBUG_COLOR Math::Vector3D(1, 0, 1)

namespace RayTracer {

AABB::AABB() {
    this->mat = std::make_shared<RayTracer::Lambertian>(DEBUG_COLOR);
}

AABB::AABB(const Interval &x, const Interval &y, const Interval &z)
    : x(x), y(y), z(z) {
    padToMinimums();
    this->mat = std::make_shared<RayTracer::Lambertian>(DEBUG_COLOR);
}

AABB::AABB(const Math::Vector3D &a, const Math::Vector3D &b) {
    x = (a.x <= b.x) ? Interval(a.x, b.x) : Interval(b.x, a.x);
    y = (a.y <= b.y) ? Interval(a.y, b.y) : Interval(b.y, a.y);
    z = (a.z <= b.z) ? Interval(a.z, b.z) : Interval(b.z, a.z);

    padToMinimums();
    this->mat = std::make_shared<RayTracer::Lambertian>(DEBUG_COLOR);
}

AABB::AABB(const AABB &box0, const AABB &box1) {
    x = Interval(box0.x, box1.x);
    y = Interval(box0.y, box1.y);
    z = Interval(box0.z, box1.z);
    this->mat = std::make_shared<RayTracer::Lambertian>(DEBUG_COLOR);
}

const Interval &AABB::axisInterval(int n) const {
    if (n == 1) return y;
    if (n == 2) return z;
    return x;
}

bool AABB::trueHit(const RayTracer::Ray &r, Interval ray_t) const {
    for (int axis = 0; axis < 3; axis++) {
        const Interval &ax = axisInterval(axis);
        const double adinv = 1.0 / *r.dir.arr[axis];

        const double t0 = (ax.min - *r.pos.arr[axis]) * adinv;
        const double t1 = (ax.max - *r.pos.arr[axis]) * adinv;

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

RayTracer::HitRecord AABB::hit(const RayTracer::Ray &r, Interval ray_t) const {
    // for (int axis = 0; axis < 3; axis++) {
    //     const Interval& ax = axisInterval(axis);
    //     const double adinv = 1.0 / *r.dir.arr[axis];

    //     auto t0 = (ax.min - *r.pos.arr[axis]) * adinv;
    //     auto t1 = (ax.max - *r.pos.arr[axis]) * adinv;

    //     if (t0 < t1) {
    //         if (t0 > ray_t.min) ray_t.min = t0;
    //         if (t1 < ray_t.max) ray_t.max = t1;
    //     } else {
    //         if (t1 > ray_t.min) ray_t.min = t1;
    //         if (t0 < ray_t.max) ray_t.max = t0;
    //     }

    //     if (ray_t.max <= ray_t.min)
    //         return HitRecord();
    // }
    // return true;

    double t_min = ray_t.min;
    double t_max = ray_t.max;
    Math::Vector3D v_min(this->x.min, this->y.min, this->z.min);
    Math::Vector3D v_max(this->x.max, this->y.max, this->z.max);

    HitRecord info;
    double t_near = t_min;
    double t_far = t_max;
    int entry_axis = -1;
    int exit_axis = -1;
    double entry_sign = 0.0;
    double exit_sign = 0.0;

    bool inside =
        r.pos.x >= v_min.x && r.pos.x <= v_max.x &&
        r.pos.y >= v_min.y && r.pos.y <= v_max.y &&
        r.pos.z >= v_min.z && r.pos.z <= v_max.z;

    for (int axis = 0; axis < 3; axis++) {
        double invD = 1.0 / *r.dir.arr[axis];
        double t0 = (*v_min.arr[axis] - *r.pos.arr[axis]) * invD;
        double t1 = (*v_max.arr[axis] - *r.pos.arr[axis]) * invD;

        double sign = invD < 0 ? -1.0 : 1.0;

        if (invD < 0.0)
            std::swap(t0, t1);

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

    Math::Vector3D normal;
    if (inside) {
        if (exit_axis == 0) normal.x = exit_sign;
        if (exit_axis == 1) normal.y = exit_sign;
        if (exit_axis == 2) normal.z = exit_sign;
    } else {
        if (entry_axis == 0) normal.x = -entry_sign;
        if (entry_axis == 1) normal.y = -entry_sign;
        if (entry_axis == 2) normal.z = -entry_sign;
    }
    bool frontFace = r.dir.dot(normal) < 0;
    info = HitRecord(inside ? t_far : t_near, r, *this, normal);
    info.mat = this->mat;
    return info;
}

// RayTracer::HitRecord AABB::hit(const RayTracer::Ray &r, Interval ray_t) const {
//     double t_min = ray_t.min;
//     double t_max = ray_t.max;
//     Math::Vector3D v_min(this->x.min, this->y.min, this->z.min);
//     Math::Vector3D v_max(this->x.max, this->y.max, this->z.max);
//     std::cout << "vmin: " << v_min << std::endl;
//     std::cout << "vmax: " << v_max<< std::endl;

//     HitRecord info;
//     double t_near = t_min;
//     double t_far = t_max;
//     int entry_axis = -1;
//     int exit_axis = -1;
//     double entry_sign = 0.0;
//     double exit_sign = 0.0;

//     bool inside =
//         r.pos.x >= v_min.x && r.pos.x <= v_max.x &&
//         r.pos.y >= v_min.y && r.pos.y <= v_max.y &&
//         r.pos.z >= v_min.z && r.pos.z <= v_max.z;

//     std::cout << std::endl;
    
//     for (int axis = 0; axis < 3; axis++) {
//         double invD = 1.0 / *r.dir.arr[axis];
//         double t0 = (*v_min.arr[axis] - *r.pos.arr[axis]) * invD;
//         double t1 = (*v_max.arr[axis] - *r.pos.arr[axis]) * invD;

//         double sign = invD < 0 ? -1.0 : 1.0;

//         if (invD < 0.0)
//             std::swap(t0, t1);

//         if (t0 > t_near) {
//             t_near = t0;
//             entry_axis = axis;
//             entry_sign = sign;
//         }

//         if (t1 < t_far) {
//             t_far = t1;
//             exit_axis = axis;
//             exit_sign = sign;
//         }

//         if (t_far <= t_near) {
//             return HitRecord();
//         }
//         std::cout << std::endl;
//         std::cout << t_min << std::endl;
//         std::cout << t_max << std::endl;
//         std::cout << t_near << std::endl;
//         std::cout << t_far << std::endl;
//         std::cout << t0 << std::endl;
//         std::cout << t1 << std::endl;
//     }

//     Math::Vector3D normal;
//     if (inside) {
//         if (exit_axis == 0) normal.x = exit_sign;
//         if (exit_axis == 1) normal.y = exit_sign;
//         if (exit_axis == 2) normal.z = exit_sign;
//     } else {
//         if (entry_axis == 0) normal.x = -entry_sign;
//         if (entry_axis == 1) normal.y = -entry_sign;
//         if (entry_axis == 2) normal.z = -entry_sign;
//     }
//     // std::cout << normal << std::endl;
//     bool frontFace = r.dir.dot(normal) < 0;
//     // std::cout << frontFace << std::endl;
//     info = HitRecord(inside ? t_far : t_near, r, *this, normal);
//     info.mat = this->mat;
//     return info;
// }

// RayTracer::HitRecord AABB::hit(const RayTracer::Ray &r, Interval ray_t) const {
//     double tmin = DOUBLE_INFINITY / 2;
//     Math::Vector3D nList[3] = {Math::Vector3D(-1, 0, 0), Math::Vector3D(0, -1, 0),
//                                Math::Vector3D(0, 0, -1)};
//     Math::Vector3D n;

//     for (int axis = 0; axis < 3; axis++) {
//         const Interval &ax = axisInterval(axis);
//         const double adinv = 1.0 / *r.dir.arr[axis];

//         double t0 = (ax.min - *r.pos.arr[axis]) * adinv;
//         double t1 = (ax.max - *r.pos.arr[axis]) * adinv;
//         Math::Vector3D faceNormalMin = Math::Vector3D(0, 0, 0);
//         Math::Vector3D faceNormalMax = Math::Vector3D(0, 0, 0);
//         *faceNormalMin.arr[axis] = 1;
//         *faceNormalMax.arr[axis] = -1;

//         if (t0 > t1) {
//             // std::swap(t0, t1);
//             std::swap(faceNormalMin, faceNormalMax);
//         }

//         if (t0 < t1) {
//             if (t0 > ray_t.min) {
//                 ray_t.min = t0;
//                 if (t0 > 0 && t0 < tmin) {
//                     tmin = t0;
//                     n = faceNormalMin;
//                 }
//             }
//             if (t1 < ray_t.max) ray_t.max = t1;
//         } else {
//             if (t1 > ray_t.min) {
//                 ray_t.min = t1;
//                 if (t1 > 0 && t1 < tmin) {
//                     tmin = t1;
//                     n = faceNormalMax;
//                 }
//             }
//             if (t0 < ray_t.max) ray_t.max = t0;
//         }

//         if (ray_t.max <= ray_t.min) return RayTracer::HitRecord();
//     }
//     HitRecord rec(tmin, r, *this, n);
//     rec.mat = this->mat;

//     return rec;
// }

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

AABB AABB::boundingBox() const {
    return *this;
}

}  // namespace RayTracer
