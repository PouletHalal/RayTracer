/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** customException base class
*/

#ifndef RAYTRACER_CORE_HPP
#define RAYTRACER_CORE_HPP

#include <SFML/Window/Keyboard.hpp>
#include <libconfig.h++>
#include <mutex>
#include <semaphore>

#include "Raytracer/math/Vector.hpp"
#include "plugins/Material.hpp"
#define CAM_SPEED 0.03
#define MAX_RAY_BOUNCE 10

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "ArgsManager.hpp"
#include "CustomException.hpp"
#include "Raytracer/Camera.hpp"
#include "Raytracer/Scene.hpp"
#include "libLoaders/ILibLoader.hpp"
#include "plugins/ILight.hpp"
#include "plugins/IShape.hpp"

class ParsingException : public CustomException {
 public:
  ParsingException(std::string message) : CustomException(message) {}
};

class RaytracerCore {
 public:
  RaytracerCore(const ArgManager::ArgumentStruct &);
  void compute(void);

  size_t getCam(void) { return currentCameraId_; };
  void setCam(size_t id) { currentCameraId_ = id; };
  std::vector<std::unique_ptr<RayTracer::Camera>> &getCamList(void) {
    return cameras_;
  };
  RayTracer::Scene &getMainScene(void) { return mainScene_; };
  double getxRes(void) { return xResolution_; };
  double getyRes(void) { return yResolution_; };
  Math::Vector3D getSize(void) { return {(double)width_, (double)height_, 0}; };
  void setMoving(bool moving) { moving_ = moving; };
  std::vector<std::uint8_t> getImageMean(void) { return imageMean_; };
  std::mutex imageMutex_;
  void writePPM(const std::string &filename);
  void loadFiles(const std::vector<std::string> &);
  void loadFile(std::string file, std::optional<RayTracer::Camera> &);
  void setCamera(RayTracer::Camera &&);

 private:
  void computeGraphic(void);
  void computeOutput(void);

    void updateBVH(void);
    std::atomic<size_t> remaingWait = 0;
    std::mutex waitingChange;

  void computeImage(size_t start, size_t end);
  void computePixel(std::vector<std::uint8_t> &image, size_t pixel,
                    size_t xResolution, size_t yResolution);

  std::vector<std::unique_ptr<RayTracer::Camera>> cameras_;
  size_t currentCameraId_ = 0;


  std::map<std::string, std::unique_ptr<LibLoader<RayTracer::IShape>>>
      shapesPlugins_;
  std::map<std::string, std::unique_ptr<LibLoader<RayTracer::ILight>>>
      lightsPlugins_;
  std::map<std::string, std::unique_ptr<LibLoader<RayTracer::Material>>>
      materials_;

  RayTracer::Scene secondScene_;
  RayTracer::Scene mainScene_;

  void initPlugins(const std::string &file, const libconfig::Config &config);
  void initMaterials(std::unique_ptr<RayTracer::IShape> &shape,
                     const libconfig::SettingIterator &shapeSetting);

  void initCamera(const std::string &file, const libconfig::Config &config,
                  std::optional<RayTracer::Camera> &camera);

  void initShape(const std::string &name, RayTracer::Scene &scene,
                 const libconfig::SettingIterator &iterator);
  void initLight(const std::string &name, RayTracer::Scene &scene,
                 const libconfig::SettingIterator &iterator);

  bool graphic_;

  std::vector<std::thread> threads_;
  bool killThreads_ = false;
  void startThreads(size_t nbThreads);

  void computeMoving(size_t start, size_t end);
  void computePrecision(void);

  size_t nbImage_ = 0;
  std::vector<int> image_;
  std::vector<std::uint8_t> imageMean_;
  size_t width_;
  size_t height_;
  size_t xResolution_;
  size_t yResolution_;

  size_t compressedXResolution_;
  size_t compressedYResolution_;
  std::vector<std::uint8_t> compressedImage_;

    bool moving_ = false;

  void handleKeys(void);
  const std::map<sf::Keyboard::Key,
                 std::function<void(RaytracerCore &, Math::Vector3D &)>>
      keyboardEvent = {
          {sf::Keyboard::Q,
           [](RaytracerCore &this_, Math::Vector3D &) {
             this_.cameras_.at(this_.currentCameraId_)
                 ->move({-CAM_SPEED, 0, 0});
           }},
          {sf::Keyboard::D,
           [](RaytracerCore &this_, Math::Vector3D &) {
             this_.cameras_.at(this_.currentCameraId_)->move({CAM_SPEED, 0, 0});
           }},
          {sf::Keyboard::Z,
           [](RaytracerCore &this_, Math::Vector3D &) {
             this_.cameras_.at(this_.currentCameraId_)->move({0, CAM_SPEED, 0});
           }},
          {sf::Keyboard::S,
           [](RaytracerCore &this_, Math::Vector3D &) {
             this_.cameras_.at(this_.currentCameraId_)
                 ->move({0, -CAM_SPEED, 0});
           }},
          {sf::Keyboard::E,
           [](RaytracerCore &this_, Math::Vector3D &) {
             this_.cameras_.at(this_.currentCameraId_)
                 ->move({0, 0, -CAM_SPEED});
           }},
          {sf::Keyboard::A,
           [](RaytracerCore &this_, Math::Vector3D &) {
             this_.cameras_.at(this_.currentCameraId_)->move({0, 0, CAM_SPEED});
           }},
          {sf::Keyboard::Left,
           [](RaytracerCore &, Math::Vector3D &camRotation) {
             camRotation.y += 0.1;
           }},
          {sf::Keyboard::Right,
           [](RaytracerCore &, Math::Vector3D &camRotation) {
             camRotation.y -= 0.1;
           }},
  };
};

#endif
