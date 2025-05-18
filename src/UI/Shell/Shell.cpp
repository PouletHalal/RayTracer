/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Shell
*/

#include "UI/Shell/Shell.hpp"

#include <format>
#include <iostream>
#include <optional>
#include <string>

#include "Raytracer/Camera.hpp"

namespace Graphics {
Shell::Shell(std::reference_wrapper<RaytracerCore> core, double width,
             double height)
    : core_(core) {
  size_ = {(float)width, (float)height};
  texture_.create(width, height);
  texture_.clear(bgcolor_);
  font_.loadFromFile("assets/fonts/roboto.ttf");
  text_.setFont(font_);
  text_.setCharacterSize(width / 20);
  text_.setFillColor(sf::Color::White);
  text_.setString(str_);
  sprite.setTexture(texture_.getTexture());
  cursor.setFillColor(sf::Color::White);
  cursor.setSize({10, size_.y / 20});
  selectedId_ = 0;
}

Shell::~Shell() {}

void Shell::updateStr(const sf::Event &events) {
  if (events.key.code >= 0 && events.key.code < 26) {
    str_ += (events.key.code + 'a');
  }
  if (events.key.code >= sf::Keyboard::Num0 &&
      events.key.code <= sf::Keyboard::Num9) {
    str_ += ('0' + (events.key.code - sf::Keyboard::Num0));
  } else if (events.key.code >= sf::Keyboard::Numpad0 &&
             events.key.code <= sf::Keyboard::Numpad9) {
    str_ += ('0' + (events.key.code - sf::Keyboard::Numpad0));
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
    if (str_.size()) str_.pop_back();
  }
  for (const auto [key, value] : KEYS) {
    if (events.key.code == key) {
      str_ += value;
    }
  }
}

std::vector<std::string> parseArgs(std::string str) {
  std::stringstream ss(str);
  std::string arg;
  std::vector<std::string> args;
  while (ss >> arg) {
    args.push_back(arg);
  }
  return args;
}

void Shell::update(const sf::Event &events) {
  updateStr(events);
  core_.get().setMoving(true);
  std::vector<std::string> args;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && str_ != "") {
    history_.push_back(str_);
    args = parseArgs(str_);
    for (auto [key, value] : functions_) {
      if (key == args[0]) {
        args.erase(args.begin());
        value(args);
      }
    }
    str_.clear();
  }
  texture_.clear(bgcolor_);
  while ((history_.size() + 2) * size_.y / 20 >= size_.y) {
    history_.erase(history_.begin());
  }
  text_.setFillColor(sf::Color::White);
  for (size_t i = 0; i < history_.size(); ++i) {
    text_.setString(history_[i]);
    text_.setPosition(0, i * size_.y / 20);
    texture_.draw(text_);
  }
  text_.setString(output_);
  text_.setPosition({0, size_.y - size_.y / 20 * 2});
  text_.setFillColor(sf::Color::Green);
  texture_.draw(text_);
  text_.setFillColor(sf::Color::White);
  if (functions_.find(str_) != functions_.end())
    text_.setFillColor(sf::Color::Green);
  text_.setString(str_);
  sf::Vector2f textsize = text_.getGlobalBounds().getSize();
  text_.setPosition({0, size_.y - size_.y / 20});
  cursor.setPosition(
      {text_.getPosition().x + textsize.x + 2, text_.getPosition().y});
  texture_.draw(text_);
  texture_.draw(cursor);
  texture_.display();
}

void Shell::select(const std::vector<std::string> &args) {
  RayTracer::Rectangle screen =
      core_.get().getCamList()[core_.get().getCam()]->screen_;
  std::reference_wrapper<RayTracer::Scene> scene =
      std::ref(core_.get().getMainScene());
  RayTracer::Ray ray = core_.get().getCamList()[core_.get().getCam()]->ray(
      screen.leftSide.y / 2, screen.bottomSide.x / 2, core_.get().getxRes(),
      core_.get().getyRes());
  RayTracer::HitRecord record;
  double closest = INFINITY;

  if (args.size()) {
    try {
      selectedId_ = std::stoi(args[0]);
      if (selectedId_ >= (int)scene.get().shapes_.size()) {
        selectedId_ = 0;
      }
      output_ = "Selected Shape n°" + std::to_string(selectedId_);
      return;
    } catch (const std::invalid_argument &) {
      return;
    }
  }
  for (size_t i = 0; i < scene.get().shapes_.size(); ++i) {
    RayTracer::HitRecord temp =
        scene.get().shapes_[i]->boundingBox().hits(ray, Interval(0, INFINITY));
    if (!temp.missed && temp.t > 0 && temp.t <= closest) {
      closest = temp.t;
      record = temp;
      selectedId_ = i;
    }
  }
  output_ = "Selected Shape n°" + std::to_string(selectedId_);
}

void Shell::move(const std::vector<std::string> &args) {
  Math::Vector3D vector;
  std::reference_wrapper<RayTracer::Scene> scene =
      std::ref(core_.get().getMainScene());

  if (args.size() < 3) return;
  try {
    vector = {std::stod(args[0]), std::stod(args[1]), std::stod(args[2])};
  } catch (const std::invalid_argument &) {
  }
  scene.get().shapes_[selectedId_]->move(vector);
  std::stringstream ss;
  output_ = "Moved Shape n° " + std::to_string(selectedId_) + "to " +
            std::format("{:.2f}", vector.x) + " " +
            std::format("{:.2f}", vector.y) + " " +
            std::format("{:.2f}", vector.z);
}

void Shell::setPos(const std::vector<std::string> &args) {
  Math::Vector3D vector;
  std::reference_wrapper<RayTracer::Scene> scene =
      std::ref(core_.get().getMainScene());

  if (args.size() < 3) return;
  try {
    vector = {std::stod(args[0]), std::stod(args[1]), std::stod(args[2])};
  } catch (const std::invalid_argument &) {
  }
  scene.get().shapes_[selectedId_]->setPosition(vector);
  output_ = "Set Shape n° " + std::to_string(selectedId_) + "position to " +
            std::format("{:.2f}", vector.x) + " " +
            std::format("{:.2f}", vector.y) + " " +
            std::format("{:.2f}", vector.z);
}

void Shell::rotate(const std::vector<std::string> &args) {
  Math::Vector3D vector;
  std::reference_wrapper<RayTracer::Scene> scene =
      std::ref(core_.get().getMainScene());

  if (args.size() < 3) return;
  try {
    vector = {std::stod(args[0]), std::stod(args[1]), std::stod(args[2])};
  } catch (const std::invalid_argument &) {
  }
  scene.get().shapes_[selectedId_]->rotate(vector);
  output_ = "Rotated Shape n° " + std::to_string(selectedId_) + "by " +
            std::format("{:.2f}", vector.x) + " " +
            std::format("{:.2f}", vector.y) + " " +
            std::format("{:.2f}", vector.z);
}

void Shell::scale(const std::vector<std::string> &args) {
  double scale = 0;
  std::reference_wrapper<RayTracer::Scene> scene =
      std::ref(core_.get().getMainScene());

  if (args.size() < 1) return;
  try {
    scale = std::stod(args[0]);
  } catch (const std::invalid_argument &) {
  }
  scene.get().shapes_[selectedId_]->scale(scale);
  output_ = "Scaled Shape n° " + std::to_string(selectedId_) + "by " +
            std::format("{:.2f}", scale);
}

void Shell::save(const std::vector<std::string> &args) {
  if (args.size() < 1) return;
  std::reference_wrapper<RayTracer::Scene> scene =
      std::ref(core_.get().getMainScene());

  libconfig::Config config;
  config.setOptions(libconfig::Config::Option::OptionAutoConvert);
  libconfig::Setting &root = config.getRoot();
  libconfig::Setting &camera =
      root.add("camera", libconfig::Setting::TypeGroup);
  libconfig::Setting &objects =
      root.add("objects", libconfig::Setting::TypeList);
  core_.get().getCamList()[core_.get().getCam()]->save(camera);
  for (const auto &it : scene.get().shapes_) {
    it->save(objects);
  }
  config.writeFile(args[0].c_str());
  output_ = "Saved Scene to" + args[0];
}

void Shell::goTo(const std::vector<std::string> &args) {
  Math::Vector3D vector;

  if (args.size() < 3) return;
  try {
    vector = {std::stod(args[0]), std::stod(args[1]), std::stod(args[2])};
  } catch (const std::invalid_argument &) {
  }
  core_.get().getCamList()[core_.get().getCam()]->setPosition(vector);
  output_ = "Moved cam to " + std::format("{:.2f}", vector.x) + " " +
            std::format("{:.2f}", vector.y) + " " +
            std::format("{:.2f}", vector.z);
}

void Shell::ppm(const std::vector<std::string> &args) {
  if (args.size() < 1) return;
  core_.get().writePPM(args[0]);
}

void Shell::cam(const std::vector<std::string> &) {
  core_.get().getCamList().push_back(std::make_unique<RayTracer::Camera>(
      *core_.get().getCamList()[core_.get().getCam()].get()));
}

void Shell::setCam(const std::vector<std::string> &args) {
  if (args.size()) {
    try {
      int id = std::stoi(args[0]);
      if (id < 0 || (size_t)id >= core_.get().getCamList().size()) {
        output_ = "INVALID ID";
        return;
      }
      core_.get().getCamList()[core_.get().getCam()]->moving_ = true;
      core_.get().getCamList()[core_.get().getCam()]->destination_ =
          core_.get().getCamList()[id]->pos_;
      core_.get().getCamList()[core_.get().getCam()]->rotationDestination_ =
          core_.get().getCamList()[id]->rotation_;
      output_ = "Current cam = " + std::to_string(id);
    } catch (const std::invalid_argument &) {
      output_ = "INVALID ID STOI";
      return;
    }
  }
}

void Shell::load(const std::vector<std::string> &args) {
  if (args.size() == 0) {
    return;
  }
  std::optional<RayTracer::Camera> hey;
  for (const auto &it : args) {
    this->core_.get().loadFile(it, hey);
  }
}

}  // namespace Graphics
