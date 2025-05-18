/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** core compute
*/

#include <unistd.h>
#include <cstdint>
#include <vector>

#include "Interval.hpp"
#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "Utils.hpp"
#include "plugins/IShape.hpp"

void RaytracerCore::updateBVH() {
    this->waitingChange.lock();
    this->remaingWait = this->threads_.size();
    int i = 0;
    while (this->remaingWait > 1) {
        usleep(100);
        i++;
        if (i > 1000) {
            break;
        }
    };
    this->mainScene_.bvh->update();
    this->waitingChange.unlock();
}

Math::Vector3D getSkyColor(const RayTracer::Ray &r) {
    Math::Vector3D unit_direction = r.dir.normalized();
    double a = 0.5 * (unit_direction.y + 1.0);
    return Math::Vector3D(0.9, 0.9, 0.9) * (1.0 - a) +
           Math::Vector3D(0.3, 0.5, 1.0) * a;
}

Math::Vector3D trace_ray(const RayTracer::Ray &r, int depth,
                         const RayTracer::Scene &scene) {
    if (depth <= 0) return Math::Vector3D(0, 0, 0);

    RayTracer::HitRecord rec = scene.hits(r, Interval(0, DOUBLE_INFINITY));

    if (rec.missed || rec.t < 1E-10) {
        return getSkyColor(r);
    }

    RayTracer::Ray scattered;
    Math::Vector3D attenuation;
    if (rec.mat->scatter(r, rec, attenuation, scattered)) {
        return (rec.mat->emitted() + (attenuation * rec.shapeHit->getPointColor(rec.p)) * trace_ray(scattered, depth - 1, scene)).clamped(0, 1);
    }
    return Math::Vector3D(0, 0, 0);
}

double linear_to_gamma(double linear_component) {
    if (linear_component > 0) return std::sqrt(linear_component);

    return 0;
}

void RaytracerCore::computePixel(std::vector<uint8_t> &image, size_t pixel,
                                 size_t xResolution, size_t yResolution) {
    size_t y = pixel / xResolution;
    size_t x = pixel % xResolution;
    double u = (double)x / xResolution;
    double v = (double)y / yResolution;
    RayTracer::Ray r = this->cameras_[currentCameraId_]->ray(u, v, xResolution, yResolution);
    RayTracer::HitRecord hitRecord;
    Math::Vector3D vec;

    vec = trace_ray(r, MAX_RAY_BOUNCE, this->mainScene_);
    vec.x = linear_to_gamma(vec.x);
    vec.y = linear_to_gamma(vec.y);
    vec.z = linear_to_gamma(vec.z);
    image[pixel * 4] = static_cast<unsigned char>(vec.x * 255);
    image[pixel * 4 + 1] = static_cast<unsigned char>(vec.y * 255);
    image[pixel * 4 + 2] = static_cast<unsigned char>(vec.z * 255);
    image[pixel * 4 + 3] = 255;
}

void RaytracerCore::compute(void) {
    if (this->graphic_) {
        this->computeGraphic();
    } else {
        this->computeOutput();
    }
}
