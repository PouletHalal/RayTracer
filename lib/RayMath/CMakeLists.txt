cmake_minimum_required(VERSION 3.10)

set(LIB_NAME rayMath)

add_library(${LIB_NAME} STATIC
    src/Vector.cpp
    src/Color.cpp
    src/Ray.cpp
    src/HitRecord.cpp
    src/Texture.cpp
    src/AShape.cpp
    src/MaterialUtils.cpp
    src/AABB.cpp
    src/Interval.cpp
    src/BVHNode.cpp
)

target_include_directories(${LIB_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)
