cmake_minimum_required(VERSION 3.0)
project(fullscreen-tools VERSION 0.3.0)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(WARNINGS 0 CACHE BOOL "Switch on extra warnings")

if(WARNINGS)
  set(WARNINGS_CXX_FLAGS
    -pedantic -Wall -Wextra -Wno-c++0x-compat -Wnon-virtual-dtor -Weffc++
    -Wconversion -Werror -Wshadow -Wcast-qual -Winit-self -Wno-unused-parameter)
  # -Winline -Wfloat-equal -Wunreachable-code
endif()

find_package(PkgConfig)
find_package(X11 REQUIRED)
find_package(Motif REQUIRED)
find_package(SDL REQUIRED)
find_package(fmt REQUIRED)
pkg_check_modules(GTKMM gtkmm-2.4 IMPORTED_TARGET)

include_directories(SYSTEM
  ${X11_INCLUDE_DIR}
  ${MOTIF_INCLUDE_DIR})

add_executable(windowhierachy windowhierachy.cpp)
target_compile_options(windowhierachy PRIVATE ${WARNINGS_CXX_FLAGS})
target_link_libraries(windowhierachy
  X11::X11
  fmt::fmt)
install(TARGETS windowhierachy
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(capturewindow capturewindow.cpp)
target_compile_options(capturewindow PRIVATE ${WARNINGS_CXX_FLAGS})
target_link_libraries(capturewindow
  ${X11_LIBRARIES}
  ${MOTIF_LIBRARIES}
  fmt::fmt)
install(TARGETS capturewindow
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

add_library(sdl-hack SHARED sdl-hack.c)
target_include_directories(sdl-hack SYSTEM PRIVATE
  ${SDL_INCLUDE_DIR})
target_link_libraries(sdl-hack
  ${SDL_LIBRARIES})
install(TARGETS sdl-hack
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(PROGRAMS sdl-hack.sh
  DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(windowtool windowtool.cpp)
target_compile_options(windowtool PRIVATE ${WARNINGS_CXX_FLAGS})
target_link_libraries(windowtool
  X11::xcb
  fmt::fmt
  PkgConfig::GTKMM)
install(TARGETS windowtool
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# EOF #
