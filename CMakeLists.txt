cmake_minimum_required(VERSION 3.14)

project(snake_cube LANGUAGES CXX)

################################################################################
# Qt
################################################################################
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)
################################################################################

################################################################################
# C++11
################################################################################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
################################################################################

################################################################################
# Sources - C++
################################################################################
file(GLOB_RECURSE snake_cube_cpp src/*.cpp)
file(GLOB_RECURSE snake_cube_h src/*.h)
################################################################################


################################################################################
# Build
################################################################################
set(PROJECT_SOURCES ${snake_cube_cpp} ${snake_cube_h})
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
    else()
        add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES}
  Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)
################################################################################

################################################################################
# Packaging (Android)
################################################################################
if(ANDROID)
  include(cmake/qt-android-mk-apk.cmake)
  qt_android_build_apk(
      TARGET ${PROJECT_NAME}
      PACKAGE_NAME fr.labedan.SnakeCube
  )
endif()
################################################################################
