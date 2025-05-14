cmake_minimum_required(VERSION 3.10)
project(RayTracer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CORE_SOURCES
    src/core/colour.cc
    src/core/image.cc
    src/core/ray.cc
    src/core/ray_tracer.cc
    src/core/scene.cc
)

set(LIGHT_SOURCES
    src/light/laser_light.cc
    src/light/point_light.cc
)

set(MATERIAL_SOURCES
    src/material/reflective.cc
    src/material/refractive.cc
    src/material/scattering.cc
)

set(SHAPES_SOURCES
    src/shapes/circle.cc
    src/shapes/wall.cc
)

set(SOURCES
    ${CORE_SOURCES}
    ${LIGHT_SOURCES}
    ${MATERIAL_SOURCES}
    ${SHAPES_SOURCES}
)

add_executable(RayTracer ${SOURCES} src/main.cc)

target_include_directories(RayTracer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

enable_testing()

add_subdirectory(thirdparty/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include)

set(TEST_SOURCES
    test/circle_test.cc
    ${CORE_SOURCES}
    ${LIGHT_SOURCES}
    ${MATERIAL_SOURCES}
    ${SHAPES_SOURCES}
)

add_executable(RayTracerTests ${TEST_SOURCES})
target_include_directories(RayTracerTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(RayTracerTests gtest gtest_main gmock)

add_test(NAME RayTracerTests COMMAND RayTracerTests)

install(TARGETS RayTracer DESTINATION bin)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -O0 -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
