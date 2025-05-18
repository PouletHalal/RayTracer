cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fPIC -O3")


add_subdirectory(Sphere)
add_subdirectory(CustomShapes)
add_subdirectory(Triangle)
add_subdirectory(Cylinder)
add_subdirectory(Lambertian)
add_subdirectory(Metal)
add_subdirectory(Plane)
add_subdirectory(Dielectric)
add_subdirectory(Cone)
add_subdirectory(Quad)
add_subdirectory(DiffuseLight)
