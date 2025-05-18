/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CustomShape
*/

#include "../include/CustomShape.hpp"

#include <fstream>
#include <libconfig.h++>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "BVHNode.hpp"
#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "plugins/IShape.hpp"
#include "plugins/Material.hpp"

namespace RayTracer {

void CustomShape::parseVertex(const std::vector<std::string> &args) {
  if (args.size() != 3) throw ParsingException("v: NOT ENOUGH COORDS");
  try {
    std::istringstream os(args[0]);
    std::istringstream os1(args[1]);
    std::istringstream os2(args[2]);
    Math::Vector3D test;
    os >> test.x;
    os1 >> test.y;
    os2 >> test.z;
    _vertices.push_back(test.rotatedX(rotation_.x).rotatedY(rotation_.y).rotatedZ(rotation_.z) * scale_);
  } catch (const std::invalid_argument &e) {
    throw ParsingException("Error parsing custom shape vertex, wrong double");
  }
}

void CustomShape::parseTexture(const std::vector<std::string> &args) {
  if (args.size() < 2) throw ParsingException("vt: NOT ENOUGH COORDS");
  try {
    std::istringstream os(args[0]);
    std::istringstream os1(args[1]);
    Math::Vector3D test;
    os >> test.x;
    os1 >> test.y;
    _textureVertices.push_back(Math::Vector3D(test));
  } catch (const std::invalid_argument &e) {
    throw ParsingException(
        "Error parsing custom shape texture vertex, wrong double");
  }
}

void CustomShape::parseNormals(const std::vector<std::string> &args) {
  if (args.size() != 3) throw ParsingException("vn: NOT ENOUGH COORDS");
  try {
    std::istringstream os(args[0]);
    std::istringstream os1(args[1]);
    std::istringstream os2(args[2]);
    Math::Vector3D test;
    os >> test.x;
    os1 >> test.y;
    os2 >> test.z;
    _normals.push_back(test);
  } catch (const std::invalid_argument &e) {
    throw ParsingException(
        "Error parsing custom shape normal vector, wrong double");
  }
}

void CustomShape::parseFace(const std::vector<std::string> &args) {
  std::vector<std::string> vectors;
  std::string tmp;
  std::stringstream stream;
  std::vector<Math::Vector3D> points;
  std::vector<Math::Vector3D> textures;
  std::vector<Math::Vector3D> normals;

  for (std::string vertex : args) {
    stream = std::stringstream(vertex);
    vectors.clear();
    while (std::getline(stream, tmp, '/')) {
      if (tmp != "") vectors.push_back(tmp);
    }
    if (_textureVertices.size())
      textures.push_back((this->_textureVertices[(std::stoi(vectors[1]) - 1) %
                                                 _textureVertices.size()]));
    points.push_back((_vertices[(std::stoi(vectors[0]) - 1)]));
    if (vectors.size() == 3)
      normals.push_back((_normals[std::stoi(vectors[2]) - 1]));
  }
  if (textures.size())
    this->textCoordinates_.push_back((textures[0] + textures[1] + textures[2]) /
                                     3);
  for (size_t i = 1; i < points.size() - 1; i++) {
    if (textures.size()) {
      Math::Vector3D avgTexture =
          (textures[0] + textures[i] + textures[i + 1]) / 3;
      this->textCoordinates_.push_back(avgTexture);
    }
    this->_faces.push_back(
        _triangleLoader
            ->getInstance<Math::Vector3D, Math::Vector3D, Math::Vector3D>(
                "value_entry_point", points[0], points[i], points[i + 1]));
  }
}

void CustomShape::parseLine(const std::string &line) {
  std::stringstream stream(line.substr(0, line.find("#")));
  std::vector<std::string> args;
  std::string type;
  std::string temp;

  stream >> type;
  while (stream >> temp) args.push_back(temp);
  if (this->_functions.find(type) != this->_functions.end())
    this->_functions.at(type)(this, args);
}

static std::unique_ptr<DlLoader<IShape>> getLoader(void) {
#if defined __linux__
  return std::make_unique<DlLoader<IShape>>("plugin/triangle");
#endif
}

void CustomShape::rotate(const Math::Vector3D &angles) {
  // Math::Vector3D toOrigin = -pos_;

  for (size_t i = 0; i < this->_faces.size(); ++i) {
    this->_faces[i]->rotate(angles);
  }
  this->rotation_ += angles;
}

void CustomShape::setPosition(const Math::Vector3D &pos) {
  this->bbox.move(pos - this->pos_);
  Math::Vector3D toPos = pos - this->pos_;
  this->move(toPos);
  this->pos_ = pos;
}

void CustomShape::move(const Math::Vector3D &offset) {
  this->bbox.move(offset);
  for (size_t i = 0; i < this->_faces.size(); ++i) {
    this->_faces[i]->move(offset);
  }
  this->pos_ += offset;
}

void CustomShape::getPos(const libconfig::Setting &settings) {
  try {
    libconfig::Setting &pos = settings.lookup("pos");
    if (!Math::lookUpVector(pos, this->pos_)) {
      throw ParsingException(
          "error parsing custom shape object, wrong \"pos\" field");
    }
  } catch (const ParsingException &e) {
    throw e;
  } catch (const libconfig::SettingNotFoundException &e) {
    throw ParsingException(e.what());
  }
  move(pos_);
}

void CustomShape::getRotation(const libconfig::Setting &settings) {
  Math::Vector3D rotation;
  try {
    libconfig::Setting &pos = settings.lookup("rotation");
    if (!Math::lookUpVector(pos, rotation)) {
      throw ParsingException(
          "error parsing custom shape object, wrong \"rotation\" field");
    }
  } catch (const ParsingException &e) {
    throw e;
  } catch (const libconfig::SettingNotFoundException &e) {
    throw ParsingException(e.what());
  }
  rotate(rotation);
}

void CustomShape::scale(size_t) {
  return;
  // Math::Vector3D pos = this->pos_;
  // for (size_t i = 0; i < faces_.size(); ++i) {
  //   faces_[i].release();
  // }
  // faces_.clear();
  // this->normals_.clear();

  // this->setPosition(Math::Vector3D());
  // for (size_t i = 0; i < vertices_.size(); ++i) {
  //   vertices_[i] *= (scale / 100);
  // }
  // for (const std::vector<std::string> &args : facesLines_) {
  //   this->parseFace(args);
  // }
  // this->setPosition(pos);
}

void CustomShape::getScale(const libconfig::Setting &settings) {
  double newscale = 0;

  if (!settings.lookupValue("scale", newscale)) {
    return;
  }
  scale_ = newscale;
  // scale(scale_);
}

CustomShape::CustomShape(const libconfig::Setting &settings) {
  std::string path;

  try {
    _triangleLoader = getLoader();
    if (!settings.lookupValue("file", path))
      throw ParsingException(
          "Error parsing custom shape, missing \"file\" field");
    std::ifstream file(path);
    std::string line;

    if (!file.is_open()) {
      throw ParsingException("error parsing custom shape, file not openned");
    }
    this->path_ = path;
    getScale(settings);
    getRotation(settings);
    while (getline(file, line)) {
      parseLine(line);
    }
    getPos(settings);
    std::string texture;
    if (settings.lookupValue("texture", texture)) {
      this->texture_ = texture;
    }
    for (size_t i = 0; i < _faces.size(); ++i) {
      this->bbox = AABB(this->bbox, _faces[i]->boundingBox());
    }
    this->bvh = std::make_unique<BVHNode>(this->_faces, 0, this->_faces.size());
  } catch (const ParsingException &e) {
    throw e;
  } catch (const libconfig::SettingNotFoundException &e) {
    throw ParsingException(e.what());
  }
}

void CustomShape::setMaterial(std::unique_ptr<Material> &material) {
  for (size_t i = 0; i < this->_faces.size(); i++) {
    std::unique_ptr<Material> tmp = material->duplicate();
    this->_faces[i]->setMaterial(tmp);
    if (this->haveTexture()) {
      const Math::Vector3D &textCoord =
          this->textCoordinates_[i % this->textCoordinates_.size()];
      this->_faces[i]->getMaterial()->setColor(
          this->texture_.getColor(textCoord.x, textCoord.y));
    }
  }
  this->material_ = std::move(material);
}

HitRecord CustomShape::hits(const Ray &ray, Interval ray_t) const {
  HitRecord record;

  if (this->bvh == nullptr) {
    throw BVHException("custom shape null bvh");
  }
  return this->bvh->hits(ray, ray_t);
}

void CustomShape::save(libconfig::Setting &parent) const {
  libconfig::Setting &shapeSettings = parent.add(libconfig::Setting::TypeGroup);
  shapeSettings.add("type", libconfig::Setting::TypeString) = "shape";
  shapeSettings.add("name", libconfig::Setting::TypeString) = "customShape";
  libconfig::Setting &data =
      shapeSettings.add("data", libconfig::Setting::TypeGroup);
      data.add("file", libconfig::Setting::TypeString) = this->path_;
    data.add("scale", libconfig::Setting::TypeFloat) = this->scale_;

  libconfig::Setting &pos = data.add("pos", libconfig::Setting::TypeGroup);
  Math::writeUpVector(pos, this->pos_);
  libconfig::Setting &rotation =
      data.add("rotation", libconfig::Setting::TypeGroup);
  Math::writeUpVector(rotation, this->rotation_);
  if (this->texture_.hasValue()) {
    data.add("texture", libconfig::Setting::TypeString) =
        this->texture_.getName();
  }
  this->material_->save(shapeSettings);
}

CustomShape::~CustomShape() {}

extern "C" {
void *entry_point(const libconfig::Setting &config) {
  return new RayTracer::CustomShape(config);
}
}
}  // namespace RayTracer
