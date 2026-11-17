cmake_minimum_required(VERSION 3.21)

project(SnakeCube VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)
qt_standard_project_setup()

qt_add_executable(SnakeCube
  src/cube.cpp
  src/cube.h
  src/glWidget.cpp
  src/glWidget.h
  src/main.cpp
  src/qHud.cpp
  src/qHud.h
)

target_link_libraries(SnakeCube PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Qt6::OpenGL
  Qt6::OpenGLWidgets
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(SnakeCube PRIVATE -Wall -Wextra)
endif()
