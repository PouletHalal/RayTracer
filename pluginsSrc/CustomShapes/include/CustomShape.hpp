/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CustomShape
*/

#ifndef CUSTOMSHAPE_HPP_
#define CUSTOMSHAPE_HPP_

#include <functional>
#include <libconfig.h++>
#include <map>

#include "BVHNode.hpp"
#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "libLoaders/LDLoader.hpp"
#include "plugins/AShape.hpp"
#include "plugins/IShape.hpp"
#include "plugins/Material.hpp"

namespace RayTracer {

class CustomShape : public AShape {
 public:
  CustomShape(const libconfig::Setting &settings);
  ~CustomShape();
  HitRecord hits(const Ray &, Interval ray_t) const override;
  void move(const Math::Vector3D &) override;
  void rotate(const Math::Vector3D &angles) override;
  void scale(size_t) override;
  void setPosition(const Math::Vector3D &) override;
  void setMaterial(std::unique_ptr<Material> &) override;
  Math::Vector3D getPointColor(const Math::Vector3D &) const override {
    return {1, 1, 1};
  };
  void save(libconfig::Setting &parent) const;

 private:
  void parseLine(const std::string &line);
  void parseVertex(const std::vector<std::string> &args);
  void parseTexture(const std::vector<std::string> &args);
  void parseNormals(const std::vector<std::string> &args);
  void parseFace(const std::vector<std::string> &args);
  void getPos(const libconfig::Setting &settings);
  void getRotation(const libconfig::Setting &settings);
  void getScale(const libconfig::Setting &settings);

  std::vector<Math::Vector3D> _vertices;
  std::vector<Math::Vector3D> _textureVertices;
  std::vector<Math::Vector3D> _normals;

  std::string path_;

  double scale_ = 1;
  Math::Vector3D pos_;
  Math::Vector3D rotation_;

  std::unique_ptr<DlLoader<IShape>> _triangleLoader;
  std::unique_ptr<BVHNode> bvh;

  std::vector<std::unique_ptr<IShape>> _faces;
  std::vector<Math::Vector3D> textCoordinates_;
  const std::map<std::string,
                 std::function<void(CustomShape *, std::vector<std::string>)>>
      _functions = {
          {"v", &CustomShape::parseVertex},
          {"vt", &CustomShape::parseTexture},
          {"vn", &CustomShape::parseNormals},
          {"f", &CustomShape::parseFace},
      };
};
}  // namespace RayTracer

#endif /* !CUSTOMSHAPE_HPP_ */
