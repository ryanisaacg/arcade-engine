cmake_minimum_required(VERSION 2.4.0)
project (arcade-engine)
set(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0015 NEW)

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
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wfatal-errors -pedantic -std=c99 -Wno-unused-parameter")
endif()

#DEPENDENCIES
add_subdirectory(data-structures)

#INCLUDES
include_directories(include)
include_directories(data-structures/include)
file(GLOB SOURCES src/*.c)
file(GLOB INCLUDES include/*.h)

#LINK LIBRARY
add_library(arcade-engine STATIC ${SOURCES} ${INCLUDES})
set_property(TARGET arcade-engine PROPERTY C_STANDARD 99)
set_target_properties(arcade-engine PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(arcade-engine m data-structures SOIL glfw3)
if(WIN32)
	target_link_libraries(arcade-engine glew32 opengl32)
else()
	target_link_libraries(arcade-engine GLEW GLU m GL m pthread dl drm Xdamage X11-xcb xcb-glx xcb-dri2 xcb-dri3 xcb-present xcb-randr xcb-xfixes xcb-render xcb-shape xcb-sync xshmfence glfw3 rt m dl Xrandr Xinerama Xxf86vm Xext Xcursor Xrender Xfixes X11 pthread xcb Xau)
endif()

#CREATE TESTS
add_executable(tests tests/tests.c)
set_property(TARGET tests PROPERTY C_STANDARD 99)
target_link_libraries(tests arcade-engine)

#EXECUTE TESTS
enable_testing()
add_test(NAME VectorLength COMMAND tests veclen)
add_test(NAME VectorScale COMMAND tests vecscl)
add_test(NAME VectorArithmetic COMMAND tests vecari)
add_test(NAME VectorRotation COMMAND tests vecrot)
add_test(NAME LineIntersect COMMAND tests lineint)
add_test(NAME RectIntersect COMMAND tests rectint)
add_test(NAME RectEngulf COMMAND tests recteng)
add_test(NAME RectOverlaps COMMAND tests rectovr)
add_test(NAME CircOverlaps COMMAND tests circovr)
add_test(NAME RectCirOverlaps COMMAND tests rectcirc)
add_test(NAME QuadTrees COMMAND tests quadtree)

#CREATE EXAMPLE
add_executable(example example.c)
set_property(TARGET example PROPERTY C_STANDARD 99)
target_link_libraries(example arcade-engine)
