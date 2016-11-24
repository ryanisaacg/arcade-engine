cmake_minimum_required(VERSION 2.4.0)
project (arcade-physics)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SOURCES src/*.cpp)
file(GLOB INCLUDES include/*.h)

add_library(arcade-physics STATIC ${SOURCES} ${INCLUDES})
set_property(TARGET arcade-physics PROPERTY C_STANDARD 99)
set_target_properties(arcade-physics PROPERTIES LINKER_LANGUAGE C)

add_executable(tests tests/tests.c)
set_property(TARGET tests PROPERTY C_STANDARD 99)
target_link_libraries(tests arcade-physics)

enable_testing()
add_test(arcade-physics-tests tests)
