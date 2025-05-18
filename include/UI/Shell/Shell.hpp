/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Shell
*/

#ifndef SHELL_HPP_
#define SHELL_HPP_

#include <SFML/Graphics.hpp>
#include <functional>

#include "RaytracerCore.hpp"

static const std::map<sf::Keyboard::Key, char> KEYS = {
    {sf::Keyboard::Apostrophe, '4'}, {sf::Keyboard::Dash, '6'},
    {sf::Keyboard::Space, ' '},      {sf::Keyboard::Slash, '-'},
    {sf::Keyboard::SemiColon, '.'},
};

namespace Graphics {
class Shell {
 public:
  Shell(std::reference_wrapper<RaytracerCore> core, double width,
        double height);
  ~Shell();
  void update(const sf::Event &events);

  void select(const std::vector<std::string> &args);
  void move(const std::vector<std::string> &args);
  void setPos(const std::vector<std::string> &args);
  void scale(const std::vector<std::string> &args);
  void rotate(const std::vector<std::string> &args);
  void save(const std::vector<std::string> &args);
  void goTo(const std::vector<std::string> &args);
  void ppm(const std::vector<std::string> &args);
  void load(const std::vector<std::string> &args);
  void cam(const std::vector<std::string> &args);
  void setCam(const std::vector<std::string> &args);

  void setState(bool state) { enabled_ = state; };
  bool getState(void) { return enabled_; };
  sf::Sprite sprite;
  std::string str_;

 protected:
  void updateStr(const sf::Event &events);

  bool enabled_ = false;

  sf::Vector2f size_;
  sf::RenderTexture texture_;
  sf::Text text_;
  sf::Font font_;
  sf::RectangleShape cursor;

  const sf::Color bgcolor_ = {10, 10, 10, 40};
  int selectedId_;
  std::vector<std::string> history_;
  std::string output_;
  std::reference_wrapper<RaytracerCore> core_;

  std::map<std::string,
           std::function<void(const std::vector<std::string> &args)>>
      functions_{
          {"select",
           [this](const std::vector<std::string> &args) { select(args); }},
          {"move",
           [this](const std::vector<std::string> &args) { move(args); }},
          {"setpos",
           [this](const std::vector<std::string> &args) { setPos(args); }},
          {"scale",
           [this](const std::vector<std::string> &args) { scale(args); }},
          {"rotate",
           [this](const std::vector<std::string> &args) { rotate(args); }},
          {"save",
           [this](const std::vector<std::string> &args) { save(args); }},
          {"goto",
           [this](const std::vector<std::string> &args) { goTo(args); }},
          {"ppm", [this](const std::vector<std::string> &args) { ppm(args); }},
          {"load",
           [this](const std::vector<std::string> &args) { load(args); }},
          {"cam", [this](const std::vector<std::string> &args) { cam(args); }},
          {"setcam",
           [this](const std::vector<std::string> &args) { setCam(args); }},
      };
};
}  // namespace Graphics

#endif /* !SHELL_HPP_ */
