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
add_subdirectory(tmx)

#INCLUDES
include_directories(include tmx/src)
file(GLOB SOURCES src/*.c)
file(GLOB INCLUDES include/*.h)

#LINK LIBRARY
add_library(arcade-engine STATIC ${SOURCES} ${INCLUDES})
set_property(TARGET arcade-engine PROPERTY C_STANDARD 99)
set_target_properties(arcade-engine PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(arcade-engine m SDL2 SDL2_image SDL2_mixer tmx xml2 z)

#CREATE TESTS
add_executable(tests tests/tests.c)
set_property(TARGET tests PROPERTY C_STANDARD 99)
target_link_libraries(tests arcade-engine)

#CREATE EXAMPLE
add_executable(example example.c)
set_property(TARGET example PROPERTY C_STANDARD 99)
target_link_libraries(example arcade-engine)
