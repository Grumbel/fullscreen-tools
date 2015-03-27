cmake_minimum_required(VERSION 3.0)
project(fullscreen-tools VERSION 0.3.0)

set(WARNINGS 0 CACHE BOOL "Switch on extra warnings")

if(WARNINGS)
  set(WARNINGS_CXX_FLAGS
    -pedantic -Wall -Wextra -Wno-c++0x-compat -Wnon-virtual-dtor -Weffc++
    -Wconversion -Werror -Wshadow -Wcast-qual -Winit-self -Wno-unused-parameter)
  # -Winline -Wfloat-equal -Wunreachable-code
endif()

find_package(X11 REQUIRED)
find_package(Motif REQUIRED)
find_package(SDL REQUIRED)

include_directories(SYSTEM
  ${X11_INCLUDE_DIR}
  ${MOTIF_INCLUDE_DIR})

add_executable(windowhierachy windowhierachy.cpp)
target_compile_options(windowhierachy PRIVATE ${WARNINGS_CXX_FLAGS})
target_link_libraries(windowhierachy
  ${X11_LIBRARIES}
  ${MOTIF_LIBRARIES})

add_executable(capturewindow capturewindow.cpp)
target_compile_options(capturewindow PRIVATE ${WARNINGS_CXX_FLAGS})
target_link_libraries(capturewindow
  ${X11_LIBRARIES}
  ${MOTIF_LIBRARIES})

add_library(sdl-hack SHARED sdl-hack.c)
target_include_directories(sdl-hack SYSTEM PRIVATE
  ${SDL_INCLUDE_DIR})
target_link_libraries(sdl-hack
  ${SDL_LIBRARIES})

# EOF #
