cmake_minimum_required(VERSION 2.4.0)
project (arcade-graphics)
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
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wfatal-errors -pedantic -std=c99")
endif()

add_subdirectory(arcade-physics)

include_directories(include arcade-physics/include arcade-physics/data-structures/include)
if(NOT WIN32)
	link_directories(/usr/local/lib64)
endif()
file(GLOB SOURCES src/*.c)

add_library(arcade-graphics STATIC ${SOURCES})
set_property(TARGET arcade-graphics PROPERTY C_STANDARD 99)
set_target_properties(arcade-graphics PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(arcade-graphics data-structures arcade-physics SOIL glfw3)
if(WIN32)
	target_link_libraries(arcade-graphics glew32 opengl32)
else()
	target_link_libraries(arcade-graphics GLEW GLU m GL m pthread dl drm Xdamage X11-xcb xcb-glx xcb-dri2 xcb-dri3 xcb-present xcb-randr xcb-xfixes xcb-render xcb-shape xcb-sync xshmfence glfw3 rt m dl Xrandr Xinerama Xxf86vm Xext Xcursor Xrender Xfixes X11 pthread xcb Xau)
endif()
