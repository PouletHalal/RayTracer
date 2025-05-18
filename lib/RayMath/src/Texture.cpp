/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Texture
*/

#include "Raytracer/Texture.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"

namespace RayTracer {

static Texture::TextureInfos getTextureInfos(const std::string &filePath,
                                             std::ifstream &stream) {
    std::string format;
    std::string bounds;
    std::string maxValue;
    Texture::TextureInfos infos;

    if (!std::getline(stream, format)) {
        throw ParsingException(filePath + ": error parsing image");
    }
    if (format != "P3") {
        throw ParsingException(filePath + ": only PPM 3 images are accepted");
    }

    if (!std::getline(stream, bounds)) {
        throw ParsingException(filePath + ": error parsing image");
    }
    std::stringstream ssBounds(bounds);
    ssBounds >> infos.width >> infos.height;
    if (ssBounds.fail()) {
        throw ParsingException(filePath + ": error parsing image bounds");
    }

    if (!std::getline(stream, maxValue)) {
        throw ParsingException(filePath + ": error parsing image");
    }
    std::stringstream ssMaxValue(maxValue);
    ssMaxValue >> infos.maxValue;
    if (ssMaxValue.fail()) {
        throw ParsingException(filePath + ": error parsing image max value");
    }
    return infos;
};

static void scaleColor(double &base, size_t max) {
    if (base > max) {
        throw ParsingException("number greater than the max value for color");
    }
    base = (base * 255) / max;
}

static Math::Vector3D getPixel(const std::string &filePath,
                               std::stringstream &ss, size_t maxValue) {
    Math::Vector3D pixel;

    ss >> pixel.x >> pixel.y >> pixel.z;
    if (ss.fail()) {
        throw ParsingException(filePath + ": failed to parse pixel");
    }
    scaleColor(pixel.x, maxValue);
    scaleColor(pixel.y, maxValue);
    scaleColor(pixel.z, maxValue);
    return pixel;
}

Texture::Texture(): image_(1, {1, {1, 1, 1}}) {}

Texture::Texture(std::string filePath): haveOne_(true), name(filePath) {
    std::ifstream stream(filePath);
    TextureInfos infos = getTextureInfos(filePath, stream);
    std::string pixels;
    try {
        pixels = std::string(std::istreambuf_iterator<char>(stream),
                             (std::istreambuf_iterator<char>()));
        std::replace(pixels.begin(), pixels.end(), '\n', ' ');
    } catch (const std::length_error &) {
        throw ParsingException(filePath + ": image is too big");
    }
    std::stringstream ssPixels(pixels);

    for (size_t y = 0; y < infos.height; y++) {
        this->image_.push_back({});
        for (size_t x = 0; x < infos.width; x++) {
            this->image_[y].push_back(
                getPixel(filePath, ssPixels, infos.maxValue));
        }
    }
}

Math::Vector3D Texture::getColor(double u, double v) const {
    size_t y = (size_t)((this->image_.size()) * v) % this->image_.size();
    size_t x = (size_t)(this->image_[y].size() * u) % this->image_[y].size();

    return this->image_[y][x];
}

bool Texture::hasValue() const {
    return this->haveOne_;
}

const std::string &Texture::getName() const {
    return this->name;
}

};  // namespace RayTracer
