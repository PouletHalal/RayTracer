/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** core compute
*/

#include <sys/types.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <functional>
#include <vector>

#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "UI/Shell/Shell.hpp"

void RaytracerCore::handleKeys() {
  Math::Vector3D camRotation;
  bool moving = false;

  for (const auto &it : this->keyboardEvent) {
    if (sf::Keyboard::isKeyPressed(it.first)) {
      it.second(*this, camRotation);
      moving = true;
    }
  }
  this->moving_ = moving;
  if (this->moving_) {
    this->imageMutex_.lock();
    this->nbImage_ = 0;
    this->imageMutex_.unlock();
  }
  this->cameras_[currentCameraId_]->rotate(camRotation);
}

void RaytracerCore::computeGraphic() {
  sf::Clock clock;
  sf::RenderWindow window(sf::VideoMode(this->width_, this->height_),
                          "Raytracer");

  sf::Texture texture;
  texture.create(this->xResolution_, this->yResolution_);
  sf::Sprite sprite(texture);
  sprite.setScale((double)this->width_ / this->xResolution_,
                  (double)this->height_ / this->yResolution_);

  sf::Texture compressedTexture;
  compressedTexture.create(this->compressedXResolution_,
                           this->compressedYResolution_);
  sf::Sprite compressedSprite(compressedTexture);
  compressedSprite.setScale(
      (double)this->width_ / this->compressedXResolution_,
      (double)this->height_ / this->compressedYResolution_);
  Graphics::Shell shell(std::ref(*this), (double)this->width_ / 3,
                        (double)this->height_ / 2);

  while (window.isOpen()) {
    if (clock.getElapsedTime().asSeconds() < 1.0 / 60.0) continue;
    clock.restart();

    cameras_[currentCameraId_]->updatePos();
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::KeyPressed) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
            sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
          shell.setState(!shell.getState());
        } else if (shell.getState()) {
          shell.update(event);
        }
      }
      if (event.type == sf::Event::Closed) window.close();
     }
    if (!shell.getState()) this->handleKeys();
    window.clear();
    if (!this->moving_ && this->nbImage_ != 0) {
      texture.update(this->imageMean_.data());
      window.draw(sprite);
    } else {
      compressedTexture.update(this->compressedImage_.data());
      window.draw(compressedSprite);
    }
    if (shell.getState()) {
      window.draw(shell.sprite);
    }
    window.display();
  }
  this->killThreads_ = true;
  for (auto &it : this->threads_) {
    it.join();
  }
}
