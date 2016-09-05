# Install script for directory: /home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/src/demo/libImage" TYPE FILE FILES
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/CMakeLists.txt"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image.hpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/sample.hpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/pixelTypes.hpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_converter.hpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_crop.hpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_io.hpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_io.cpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_drawing.hpp"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/src/demo/libImage" TYPE FILE FILES
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_test.cpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_drawing_test.cpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_io_test.cpp"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/src/demo/libImage/image_test" TYPE FILE FILES
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_test/two_pixels_color.jpg"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_test/two_pixels_monochrome.jpg"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_test/two_pixels.ppm"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_test/two_pixels_color.png"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_test/two_pixels_monochrome.png"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_test/two_pixels_gray.pgm"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_test/two_pixels.pgm"
    )
endif()

