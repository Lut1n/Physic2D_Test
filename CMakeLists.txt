cmake_minimum_required(VERSION 3.5)

project(Physic2D_Test)

set(SRCS
    main.cpp
    renderer.cpp
    physics/physic_engine.cpp
    physics/physic_entity.cpp
    maths/math_intersection.cpp
    maths/math_geometry.cpp
    maths/math_vector.cpp
    )

set(HEADERS
    renderer.hpp
    physics/physic_engine.hpp
    physics/physic_entity.hpp
    maths/math_intersection.hpp
    maths/math_geometry.hpp
    maths/math_vector.hpp
    )

add_executable(PhysicTest ${SRCS} ${HEADERS})

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
# set(SFML_DIR "<sfml root prefix>/lib/cmake/SFML")
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

target_link_libraries(PhysicTest sfml-graphics)
