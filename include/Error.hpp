/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Error
*/

#ifndef ERROR_HPP
#define ERROR_HPP

#include <exception>
#include <string>

namespace RayTracer {

class RayTracerError : public std::exception {
   private:
    std::string _msg;

   public:
    RayTracerError(std::string message) : _msg(message) {};
    const char *what() const noexcept override {
        return this->_msg.c_str();
    }
};

}  // namespace RayTracer

#endif
