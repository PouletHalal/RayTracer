/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** BVHNode
*/

#ifndef BVH_HPP
#define BVH_HPP

#include <algorithm>

#include "FixCrossInclude.hpp"
#include "AABB.hpp"
#include "AShape.hpp"
#include "HitRecord.hpp"
#include "Ray.hpp"
#include "Scene.hpp"

class BVHNode : public RayTracer::AShape {
   public:

    BVHNode(RayTracer::Scene list, int depth)
        : BVHNode(list.shapeList, 0, list.shapeList.size()) {
            this->depth = depth;
    }

    BVHNode(std::vector<std::shared_ptr<IShape>> &objects, size_t start,
            size_t end) {
        this->bbox = RayTracer::AABB(Interval(+DOUBLE_INFINITY, -DOUBLE_INFINITY),
                    Interval(+DOUBLE_INFINITY, -DOUBLE_INFINITY),
                    Interval(+DOUBLE_INFINITY, -DOUBLE_INFINITY));
        for (size_t object_index = start; object_index < end; object_index++)
            this->bbox = RayTracer::AABB(this->bbox, objects[object_index]->boundingBox());

        int axis = this->bbox.longestAxis();
        bool (*comparator)(const std::shared_ptr<IShape> a,
                           const std::shared_ptr<IShape> b) = this->comp[axis];

        size_t object_span = end - start;

        if (object_span == 1) {
            this->left = objects[start];
            this->right = objects[start];
        } else if (object_span == 2) {
            this->left = objects[start];
            this->right = objects[start + 1];
        } else {
            std::sort(std::begin(objects) + start, std::begin(objects) + end,
                      comparator);

            auto mid = start + object_span / 2;
            this->left = std::make_shared<BVHNode>(objects, start, mid);
            this->right = std::make_shared<BVHNode>(objects, mid, end);
        }

        this->bbox = RayTracer::AABB(this->left->boundingBox(), this->right->boundingBox());
    }

    RayTracer::HitRecord hit(const RayTracer::Ray &r,
                             Interval ray_t) const override {
        // if (this->depth <= 0) {
        //     // printf("[DEBUG]\n");
        //     return this->bbox.hit(r, ray_t);
        // }
        // if (bbox.hit(r, ray_t).missed) return RayTracer::HitRecord();

        // return bbox.hit(r, ray_t);
        this->left->depth = this->depth - 1;
        this->right->depth = this->depth - 1;
        RayTracer::HitRecord hit_left = this->left->hit(r, ray_t);
        RayTracer::HitRecord hit_right = this->right->hit(
            r, Interval(ray_t.min, hit_left.missed ? ray_t.max : hit_left.t));

        return hit_right.missed ? hit_left : hit_right;
    }

    static bool box_compare(const std::shared_ptr<IShape> a,
                            const std::shared_ptr<IShape> b, int axis_index) {
        Interval a_axis_interval = a->boundingBox().axisInterval(axis_index);
        Interval b_axis_interval = b->boundingBox().axisInterval(axis_index);
        return a_axis_interval.min < b_axis_interval.min;
    }

    static bool box_x_compare(const std::shared_ptr<IShape> a,
                              const std::shared_ptr<IShape> b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(const std::shared_ptr<IShape> a,
                              const std::shared_ptr<IShape> b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(const std::shared_ptr<IShape> a,
                              const std::shared_ptr<IShape> b) {
        return box_compare(a, b, 2);
    }

   private:
    std::shared_ptr<IShape> left;
    std::shared_ptr<IShape> right;
    bool (*comp[3])(const std::shared_ptr<IShape> a,
                    const std::shared_ptr<IShape> b) = {
        box_x_compare, box_y_compare, box_z_compare};
};

#endif
