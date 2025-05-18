/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** core constructor
*/

#include <unistd.h>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <libconfig.h++>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

#include "../../include/BVHNode.hpp"
#include "Raytracer/Camera.hpp"
#include "Raytracer/Scene.hpp"
#include "Raytracer/Texture.hpp"
#include "RaytracerCore.hpp"
#include "libLoaders/ILibLoader.hpp"
#include "plugins/IShape.hpp"
#include "plugins/Material.hpp"

#if defined __linux__
#include "libLoaders/LDLoader.hpp"
#endif
#if defined _WIN32
#include "libLoaders/WindowsLoader.hpp"
#endif

#include "plugins/ILight.hpp"

template <typename Plugin>
static void initInMap(
    std::map<std::string, std::unique_ptr<LibLoader<Plugin>>> &map,
    const std::string &name) {
    if (map.contains(name)) {
        return;
    }
    try {
// LINUX
#if defined __linux__
        map.insert(
            {name, std::make_unique<DlLoader<Plugin>>("plugin/" + name)});
#endif
//
// WINDOWS
#if defined _WIN32
        map.insert(
            {name, std::make_unique<WindowsLoader<Plugin>>("plugin/" + name)});
#endif
        //
    } catch (const LoaderException &ex) {
        throw ParsingException("error loading pluggin :" +
                               std::string(ex.what()));
    }
}

void RaytracerCore::initCamera(const std::string &file,
                               const libconfig::Config &config,
                               std::optional<RayTracer::Camera> &camera) {
    try {
        libconfig::Setting &cameraSettings = config.lookup("camera");
        if (camera.has_value()) {
            throw ParsingException(
                "camera object cannot be declared twice. redeclared in file " +
                file + ", ignored.");
        }
        camera.emplace(cameraSettings);
    } catch (const libconfig::SettingNotFoundException &) {
    } catch (const ParsingException &exc) {
        std::cerr << exc.what() << std::endl;
    }
}

void RaytracerCore::initShape(const std::string &name, RayTracer::Scene &scene,
                              const libconfig::SettingIterator &iterator) {
    initInMap<RayTracer::IShape>(this->shapesPlugins_, name);
    std::unique_ptr<RayTracer::IShape> shape;

    try {
        shape = this->shapesPlugins_.at(name)->getInstance(
            "entry_point", iterator->lookup("data"));
    } catch (const std::out_of_range &exp) {
        throw ParsingException("shape \"" + name + "\" does not exist");
    } catch (const LoaderException &exc) {
        throw ParsingException("error loading plugin: " +
                               std::string(exc.what()));
    }
    try {
        this->initMaterials(shape, iterator);
    } catch (const ParsingException &exc) {
        throw ParsingException("failed to init material for object \"" + name +
                               "\": " + exc.what());
        return;
    }
    // shape->setBoundingBox(RayTracer::AABB(Math::Vector3D(-100000, -100000, -100000), Math::Vector3D(100000, 100000, 100000)));
    scene.addShape(std::move(shape));
}

void RaytracerCore::initLight(const std::string &name, RayTracer::Scene &scene,
                              const libconfig::SettingIterator &iterator) {
    initInMap<RayTracer::ILight>(this->lightsPlugins_, name);
    try {
        scene.addLight(this->lightsPlugins_.at(name)->getInstance(
            "entry_point", iterator->lookup("data")));
    } catch (const std::out_of_range &) {
        throw ParsingException("light \"" + name + "\" does not exist");
    } catch (const libconfig::SettingNotFoundException &) {
        throw ParsingException("missing or invalid \"data\" field");
    } catch (const LoaderException &exc) {
        throw ParsingException("error loading plugin: " +
                               std::string(exc.what()));
    }
}

void RaytracerCore::initMaterials(
    std::unique_ptr<RayTracer::IShape> &shape,
    const libconfig::SettingIterator &shapeSetting) {
    libconfig::Setting &material = shapeSetting->lookup("material");
    std::string materialType;
    if (!material.lookupValue("type", materialType)) {
        throw ParsingException("missing or invalid type field");
    }
    initInMap(this->materials_, materialType);
    try {
        std::unique_ptr<RayTracer::Material> tmp =
            this->materials_.at(materialType)
                ->getInstance("entry_point", material.lookup("data"));
        shape->setMaterial(tmp);
    } catch (const std::out_of_range &) {
        throw ParsingException("material \"" + materialType +
                               "\" does not exist");
    } catch (const LoaderException &exc) {
        throw ParsingException("error loading plugin: " +
                               std::string(exc.what()));
    }
}

void RaytracerCore::initPlugins(const std::string &file,
                                const libconfig::Config &config) {
    try {
        libconfig::Setting &objects = config.lookup("objects");
        for (auto i = objects.begin(); i != objects.end(); i++) {
            RayTracer::Scene scene;
            for (auto it = i->begin(); it != i->end(); it++) {
                try {
                    std::string type;
                    std::string name;
                    if (!it->lookupValue("type", type)) {
                        throw ParsingException(
                            "missing or invalid \"type\" field");
                    }
                    if (!it->lookupValue("name", name)) {
                        throw ParsingException(
                            "missing or invalid \"name\" field");
                    }
                    if (type == "shape") {
                        this->initShape(name, scene, it);
                    } else if (type == "light") {
                        this->initLight(name, scene, it);
                    } else {
                        throw ParsingException("object type \"" + type +
                                               "\" does not exist");
                    }
                } catch (const ParsingException &exc) {
                    std::cerr << "error initializing object in file \"" << file
                              << "\": " << exc.what() << std::endl;
                }
            }
            if (scene.shapes_.empty()) {
                continue;
            }
            scene.bvh = std::make_unique<RayTracer::BVHNode>(
                scene.shapes_, 0, scene.shapes_.size());
            scene.bvh->parentObject = &this->mainScene_;
            this->mainScene_.addShape(
                std::make_unique<RayTracer::Scene>(std::move(scene)));
        }
    } catch (const libconfig::SettingNotFoundException &) {
    }
}

void RaytracerCore::startThreads(size_t nbThreads) {
    size_t start = 0;
    size_t nbPixelPerThread =
        (this->compressedXResolution_ * this->compressedYResolution_) /
        nbThreads;
    size_t end = nbPixelPerThread;
    for (size_t i = 0; i < nbThreads; i++) {
        if (i + 1 == nbThreads) {
            end = this->compressedXResolution_ * this->compressedYResolution_;
        }
        this->threads_.push_back(
            std::thread(&RaytracerCore::computeImage, this, start, end));
        start += nbPixelPerThread;
        end += nbPixelPerThread;
    }
}

void RaytracerCore::loadFile(std::string file, std::optional<RayTracer::Camera> &camera) {
    libconfig::Config config;

    config.setOptions(libconfig::Config::OptionAutoConvert |
                      libconfig::Config::OptionAllowScientificNotation);
    try {
        config.readFile(file);
        this->initCamera(file, config, camera);
        this->initPlugins(file, config);
        if (this->mainScene_.shapes_.empty()) {
            throw ParsingException("empty scene");
        }
        this->mainScene_.bvh = std::make_unique<RayTracer::BVHNode>(this->mainScene_.shapes_, 0, this->mainScene_.shapes_.size());
        this->mainScene_.bvh->parentObject = nullptr;
    } catch (const libconfig::FileIOException &exc) {
        std::cerr << "error parsing file \"" << file
                  << "\", failed to open file" << std::endl;
    } catch (const libconfig::ParseException &) {
        std::cerr << "error parsing file \"" << file << "\", invalid format"
                  << std::endl;
    }
}

void RaytracerCore::loadFiles(const std::vector<std::string> &files) {
    std::optional<RayTracer::Camera> camera = std::nullopt;

    this->imageMutex_.lock();
    this->moving_ = true;
    sleep(1);
    this->mainScene_.shapes_.clear();

    this->nbImage_ = 0;
    for (const std::string &file : files) {
        this->loadFile(file, camera);
    }
    if (camera.has_value()) {
        this->setCamera(std::move(camera.value()));
    } else {
        throw ParsingException("missing camera");
    }
    this->moving_ = false;
    this->imageMutex_.unlock();
}

void RaytracerCore::setCamera(RayTracer::Camera &&cam) {
    this->cameras_.push_back(std::make_unique<RayTracer::Camera>((cam)));

    std::unique_ptr<RayTracer::Camera> &cam1 = cameras_[currentCameraId_];
    cameras_[currentCameraId_]->screen_.bottomSide.x =
     cameras_[currentCameraId_]->screen_.leftSide.y * (double) xResolution_ / yResolution_;
    cam1->screen_.pos = {cam1->pos_.x - cam1->screen_.bottomSide.x / 2, cam1->pos_.y - cam1->screen_.leftSide.y / 2, cam1->pos_.z - 1.5};
}

RaytracerCore::RaytracerCore(const ArgManager::ArgumentStruct &args)
    : graphic_(args.graphicMode),
      nbImage_(0),
      image_(args.xResolution * args.yResolution * 4, 0),
      imageMean_(args.xResolution * args.yResolution * 4, 0),
      width_(args.width),
      height_(args.height),
      xResolution_(args.xResolution),
      yResolution_(args.yResolution),
      compressedXResolution_(std::max((int)(args.xResolution / 10), 20)),
      compressedYResolution_(std::max((int)args.yResolution / 10, 20)),
      compressedImage_(
          this->compressedXResolution_ * this->compressedYResolution_ * 4, 0) {
    this->loadFiles(args.configFile);
    this->startThreads(args.nb_thread);
}
