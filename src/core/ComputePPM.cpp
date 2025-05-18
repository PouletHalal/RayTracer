/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** core compute
*/

#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>

#include "CustomException.hpp"
#include "RaytracerCore.hpp"

void RaytracerCore::writePPM(const std::string &filename) {
  std::ofstream file(filename);

  if (!file.is_open()) {
    throw CustomException("failed to create file for ppm output");
  }
  while (this->nbImage_ == 0);
  std::string header =
      std::format("P3\n{} {}\n255\n", this->xResolution_, this->yResolution_);
  file.write(header.c_str(), header.length());
  for (size_t y = 0; y < this->yResolution_; y++) {
    for (size_t x = 0; x < this->xResolution_; x++) {
      std::uint8_t *pixel =
          this->imageMean_.data() + (y * this->xResolution_ + x) * 4;
      file << (int)(pixel[0]) << " " << (int)(pixel[1]) << " "
           << (int)(pixel[2]) << "\n";
    }
  }
}

void RaytracerCore::computeOutput() {
    int lastNbImage = -1;
    while (this->nbImage_ < 200) {
        usleep(500000);
        std::cout << "\33[2K\r" << "computing images :" << this->nbImage_ << "/200 [";
        lastNbImage = this->nbImage_;
        for (int i = 0; i < 200; i+= 2) {
            std::cout << ((i < lastNbImage) ? "\033[32m|" : "\033[0m ") << std::flush;
        }
        std::cout << "\033[0m]" << std::flush;
    }
    for (auto &it : this->threads_) {
        it.join();
    }
    this->writePPM("output.ppm");
}
