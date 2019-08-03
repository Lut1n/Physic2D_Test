cmake_minimum_required(VERSION 3.5)

project(Physic2D_Test)

set(SRCS
    main.cpp
    physic_engine.cpp
    physic_entity.cpp
    math_intersection.cpp
    math_geometry.cpp
    math_vector.cpp
    )

set(HEADERS
    physic_engine.hpp
    physic_entity.hpp
    math_intersection.hpp
    math_geometry.hpp
    math_vector.hpp
    )

add_executable(PhysicTest ${SRCS} ${HEADERS})

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
# set(SFML_DIR "<sfml root prefix>/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

target_link_libraries(PhysicTest sfml-graphics)
