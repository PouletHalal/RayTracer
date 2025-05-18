cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME raytracer)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugin)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(INCLUDES_DIRECTORY ${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fPIC -O3")

add_subdirectory(lib/RayMath)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/ArgsManager.cpp
    src/Raytracer/Camera.cpp
    src/Raytracer/Rectangle.cpp
    src/Raytracer/Scene.cpp
    src/core/Constructor.cpp
    src/core/Compute.cpp
    src/core/Threads.cpp
    src/core/ComputeGraphic.cpp
    src/core/ComputePPM.cpp
    src/UI/Shell/Shell.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${INCLUDES_DIRECTORY}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    rayMath
    config++
    sfml-graphics
    sfml-window
    sfml-system
)

add_custom_target(re
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMENT "Rebuilding the project"
)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3")

add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --config Debug
    COMMENT "Rebuilding the project in Debug mode"
)

add_custom_target(doc
    COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/doc
    COMMAND doxygen ${CMAKE_SOURCE_DIR}/doxygen_config.txt
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating documentation with Doxygen in doc folder"
)

add_subdirectory(pluginsSrc)
