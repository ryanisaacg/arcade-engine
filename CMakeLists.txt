cmake_minimum_required(VERSION 2.4.0)
project (arcade-physics)
set(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0003 NEW)

#SET WARNINGS
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC)
  # Update if necessary
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wfatal-errors -pedantic -std=c99")
endif()

#DEPENDENCIES
add_subdirectory(data-structures)

#INCLUDES
include_directories(include)
include_directories(data-structures/include)
file(GLOB SOURCES src/*.c)
file(GLOB INCLUDES include/*.h)

#LINK LIBRARY
add_library(arcade-physics STATIC ${SOURCES} ${INCLUDES})
set_property(TARGET arcade-physics PROPERTY C_STANDARD 99)
set_target_properties(arcade-physics PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(arcade-physics m)
target_link_libraries(arcade-physics data-structures)

#CREATE TESTS
add_executable(tests tests/tests.c)
set_property(TARGET tests PROPERTY C_STANDARD 99)
target_link_libraries(tests arcade-physics)

#EXECUTE TESTS
enable_testing()
add_test(VectorLength tests veclen)
add_test(VectorScale tests vecscl)
add_test(VectorArithmetic tests vecari)
add_test(VectorRotation tests vecrot)
add_test(LineIntersect tests lineint)
add_test(RectIntersect tests rectint)
add_test(RectEngulf tests recteng)
add_test(RectOverlaps tests rectovr)
add_test(CircOverlaps tests circovr)
add_test(RectCirOverlaps test rectcirc)
add_test(QuadTrees test quadtree)
