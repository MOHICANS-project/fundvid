# Install script for directory: /home/nicola/Downloads/orsa-fundamental_1.0/src/third_party/CppUnitLite

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/src/third_party/CppUnitLite" TYPE FILE FILES
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/third_party/CppUnitLite/CMakeLists.txt"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/third_party/CppUnitLite/Test.h"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/third_party/CppUnitLite/TestHarness.h"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/third_party/CppUnitLite/Failure.h"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/third_party/CppUnitLite/TestRegistry.h"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/third_party/CppUnitLite/TestResult.h"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/third_party/CppUnitLite/SimpleString.h"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/third_party/CppUnitLite/TestRegistry.cpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/third_party/CppUnitLite/Failure.cpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/third_party/CppUnitLite/TestResult.cpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/third_party/CppUnitLite/SimpleString.cpp"
    "/home/nicola/Downloads/orsa-fundamental_1.0/src/third_party/CppUnitLite/Test.cpp"
    )
endif()

