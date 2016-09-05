# Install script for directory: /home/nicola/Downloads/orsa-fundamental_1.0/src/extras/sift

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/src/extras/sift" TYPE FILE FILES
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/extras/sift/CMakeLists.txt"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/extras/sift/library.cpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/extras/sift/library.h"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/extras/sift/domain.cpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/extras/sift/domain.h"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/extras/sift/splines.cpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/extras/sift/splines.h"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/extras/sift/filter.cpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/extras/sift/filter.h"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/extras/sift/demo_lib_sift.cpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/extras/sift/demo_lib_sift.h"
    )
endif()

