cmake_minimum_required(VERSION 2.6)
#Configuration du projet
project(multicamnet)


#############################################
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

ADD_DEFINITIONS(
	-std=c++11
	#CMAKE_CXX_COMPILER=g++-4.8
)


############################################

#find_package(Qt4 4.4.3 COMPONENTS QtCore QtGui QtXml REQUIRED )
#INCLUDE_DIRECTORIES(/usr/include/qt4) 

#############################################

ADD_DEFINITIONS(-DDEBUG -ggdb -O3 -Wall -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -fbranch-probabilities)
SET( EIGEN3_INCLUDE_DIR "/usr/include/eigen3" )
IF( NOT EIGEN3_INCLUDE_DIR )
    MESSAGE( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
ENDIF()
INCLUDE_DIRECTORIES ( "${EIGEN3_INCLUDE_DIR}" )
if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


LINK_DIRECTORIES(${CMAKE_BINARY_DIR}) 

include_directories(
	${CONFIG++_INCLUDE_DIR}
)
link_directories(
	${LAPACK_LIBRARY_DIRS}
)
#############################################


############################ LEVMAR ########################################

# levmar library
SET(LEVMAR_INCDIR "/usr/lib/levmar-2.6" CACHE PATH "Path to LEVMAR library header")
SET(LEVMAR_LIBDIR "/usr/lib/levmar-2.6" CACHE PATH "Path to LEVMAR library")

# lapack/blas
SET(HAVE_LAPACK 1 CACHE BOOL "Do we have LAPACK/BLAS?")
# the directory where the lapack/blas/f2c libraries reside
SET(LAPACKBLAS_DIR "/usr/lib" CACHE PATH "Path to lapack/blas libraries")
#SET(LAPACKBLAS_DIR "C:\Program Files\Intel\MKL\10.2.2.025\ia32\lib" CACHE PATH "Path to lapack/blas libraries") # MKL
SET(NEED_F2C 1 CACHE BOOL "Do we need either f2c or F77/I77?")
SET(HAVE_PLASMA 0 CACHE BOOL "Do we have PLASMA parallel linear algebra library?")
IF(HAVE_PLASMA)
 SET(PLASMA_DIR "/usr/local/PLASMA" CACHE PATH "Path to PLASMA root")
ENDIF(HAVE_PLASMA)
OPTION(BUILD_DEMO "Build demo program?" TRUE)

# actual names for the lapack/blas/f2c libraries
SET(LAPACKBLAS_LIB_NAMES "lapack;blas" CACHE STRING "The name of the lapack & blas libraries")
#SET(LAPACKBLAS_LIB_NAMES "mkl_solver_sequential;mkl_intel_c;mkl_sequential;mkl_core" CACHE STRING "The name of the lapack libraries") # MKL
IF(NEED_F2C)
  SET(F2C_LIB_NAME f2c CACHE STRING "The name of the f2c or F77/I77 library")
   #f2c is sometimes equivalent to libF77 & libI77
  SET(F2C_LIB_NAME "libF77;libI77" CACHE STRING "The name of the f2c or F77/I77 library")
ELSE(NEED_F2C)
  SET(F2C_LIB_NAME "" CACHE STRING "The name of the f2c or F77/I77 library")
ENDIF(NEED_F2C)

# actual names for the PLASMA libraries
IF(HAVE_PLASMA)
  SET(PLASMA_LIB_NAMES "plasma;coreblas;quark;lapacke" CACHE STRING "The names of the PLASMA libraries")
ENDIF(HAVE_PLASMA)

SET(HAVE_HOMEST 0 CACHE BOOL "Do we have HOMEST installed? (for GRIC only)")
IF(HAVE_HOMEST)
 SET(HOMEST_INCDIR "/usr/local/homest-1.4" CACHE PATH "Path to HOMEST library header")
 SET(HOMEST_LIBDIR "/usr/local/homest-1.4" CACHE PATH "Path to HOMEST library")
 ADD_DEFINITIONS(-DHAVE_HOMEST)
ENDIF(HAVE_HOMEST)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${LEVMAR_INCDIR})
INCLUDE_DIRECTORIES(${HOMEST_INCDIR})


IF(HAVE_PLASMA)
 INCLUDE_DIRECTORIES(${PLASMA_DIR}/include ${PLASMA_DIR}/quark)
ENDIF(HAVE_PLASMA)

SET(LIBS ${LIBS} levmar)

  LINK_DIRECTORIES(${CMAKE_BINARY_DIR}) # location of the fundest library
  LINK_DIRECTORIES(${LAPACKBLAS_DIR})
  LINK_DIRECTORIES(${LEVMAR_LIBDIR})

# libraries the demo depends on
  IF(HAVE_PLASMA)
    LINK_DIRECTORIES(${PLASMA_DIR}/lib)
    SET(LIBS ${LIBS} ${PLASMA_LIB_NAMES})
  ENDIF(HAVE_PLASMA)
  SET(LIBS ${LIBS} ${LAPACKBLAS_LIB_NAMES})

####################################################################

#include(${QT_USE_FILE})
set(EXECUTABLE_OUTPUT_PATH bin)

## Checking library....

#OpenCV
Find_Package(OpenCV REQUIRED)
if ( NOT OpenCV_FOUND )
    message(FATAL_ERROR "OpenCV not found !")
endif ( NOT OpenCV_FOUND )

#Boost
Find_Package(Boost REQUIRED system filesystem)
if ( NOT Boost_FOUND )
    message(FATAL_ERROR "Boost not found !")
endif ( NOT Boost_FOUND )
set(Boost_DIR $ENV{BOOST_ROOT})

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)


#Compilation flags
set(CMAKE_CXX_FLAGS "-Wall -O3 -mssse3 -fpermissive -pthread")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DENABLE_DEBUG")

#Configuration de l'exécutable

#file(
#	GLOB_RECURSE
#	source_files
#	src/*
#) 

add_definitions(-DVL_DISABLE_AVX=1)

include_directories(src/reader)
include_directories(src/test)
include_directories(src/matchers)
include_directories(src/estimation)
include_directories(src/exceptions)
include_directories(src/third_party/orsa-fundamental_1.0/src)
add_subdirectory(src/third_party/orsa-fundamental_1.0/src)
######################################

file(GLOB IO  src/reader/imagesreader.cpp src/reader/boostreader.cpp)
file(GLOB Match src/matchers/sift_opencv_matcher.cpp src/matchers/sift_guided_matching.cpp)
file(GLOB Misc src/exceptions/optimization_exception.cpp src/estimation/orsa_estimator.cpp src/estimation/lm_fundmat_optimizer.cpp src/estimation/f_uncertainty.cpp src/fundamentalmat_solver.cpp)

ADD_LIBRARY(myIO SHARED ${IO})
ADD_LIBRARY(myMatch SHARED ${Match})
ADD_LIBRARY(myMisc SHARED ${Misc} src/sigma_functions/sigma_step_function.cpp src/sigma_functions/sigma_step_function.h src/sigma_functions/sigma_function.h src/sigma_functions/sigma_sigmoid_function.cpp src/sigma_functions/sigma_sigmoid_function.h src/density_estimator/density_estimator.h src/density_estimator/kernel_density_estimator.cpp src/density_estimator/kernel_density_estimator.h src/kernels/kernel.h src/kernels/epanechnikov_kernel.cpp src/kernels/epanechnikov_kernel.h src/kernels/histogram_kernel.cpp src/kernels/histogram_kernel.h)

ADD_EXECUTABLE(
  estimate_fundamental
  src/main.cpp
)

ADD_EXECUTABLE(
        density_test
        src/test/density_test.cpp
        src/kernels/kernel.h)

ADD_EXECUTABLE(
        density_map_test
        src/test/density_map_test.cpp)

ADD_EXECUTABLE(
        matchPointsXml
        src/gt_annotation/matchPointsXml.cc)

TARGET_LINK_LIBRARIES(
  estimate_fundamental
  myIO
  myMatch
  myMisc
  Orsa
  levmar
  GVars3
  ${CONFIG++_LIBRARY}
  ${EXTRA_LIBS}
  ${LIBS}
  ${OPENCV_LIBRARIES}
  ${OpenCV_LIBS}
  boost_system
  boost_filesystem
)

TARGET_LINK_LIBRARIES(
        density_map_test
        myMisc
        Orsa
        levmar
        GVars3
        ${CONFIG++_LIBRARY}
        ${EXTRA_LIBS}
        ${LIBS}
        ${OPENCV_LIBRARIES}
        ${OpenCV_LIBS}
        myIO
        myMatch
)

TARGET_LINK_LIBRARIES(
        matchPointsXml
        ${OPENCV_LIBRARIES}
        ${OpenCV_LIBS}
)

foreach(dir ${DIRS})
  FILE(GLOB files ${dir}/*.h*)
  LIST(APPEND h_files ${files})
endforeach(dir)
add_custom_target(TAGS etags --members -l c++ ${h_files} ${simple_SRC})
