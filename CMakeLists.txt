# CMakeLists files in this project can refer to the root source directory of
# the project as ${SOURCE_DIR} and to the root binary directory of
# the project as ${BINARY_DIR}.
cmake_minimum_required(VERSION 2.8)

#note: use correct gcc version /usr/bin/gcc
PROJECT(CERT_CHECK)

# include standard cmake modules
INCLUDE(FindThreads)
#INCLUDE(FindQt4)

# Allow empty ELSE() and ENDIF() constructs
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)
# gnu-compiler flags

# Version info
SET(MAJOR_VERSION 0)
SET(MINOR_VERSION 1)
SET(PATCH_VERSION 0)

SET(CMAKE_C_FLAGS "-std=c99 -pedantic")
SET(CMAKE_CXX_FLAGS "-std=c++11 -pedantic")


ADD_DEFINITIONS( 
  #          -finline-functions -O3  -fomit-frame-pointer  -momit-leaf-frame-pointer 
      -fstrict-aliasing
      -W -Wall -Wextra -Wno-long-long
     -Wno-sign-compare -Wno-unused-parameter -Wno-unused-label -Wno-cast-qual
     -Wno-char-subscripts 
     -Wunused-function   -Wunused-value -Wunused-variable
  -Wpointer-arith  -Wcast-align -Wwrite-strings
  #out: -Wconversion 
  )


IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: 'Debug' and 'Release'."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

IF(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR CMAKE_BUILD_TYPE STREQUAL "debug")
    ADD_DEFINITIONS(-g)
    SET(DEBUG_STRING _d)
ENDIF()

LINK_DIRECTORIES(../third_party_libs/openssl-1.1.0
  ../third_party_libs/Botan ../third_party_libs/mbedtls-2.4.2/library  )


SET(PROJECT_BINARY_DIR build)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})


# Directories
SET(INCLUDE_DIR include 
  ../third_party_libs/openssl-1.1.0/include
  ../third_party_libs/Botan-2.2.0/build/include
  ../third_party_libs/mbedtls-2.4.2/include )

SET(SRC_DIR src )



# List of source files
FILE(GLOB SRCS ${SRC_DIR}/*.cpp ${SRC_DIR}/*.c )

# include folder with header files
INCLUDE_DIRECTORIES(${INCLUDE_DIR} )

ADD_EXECUTABLE(cert_test ${SRCS})
#TARGET_LINK_LIBRARIES(cert_test crypto)
TARGET_LINK_LIBRARIES(cert_test crypto botan-2 mbedcrypto mbedx509 mbedtls)


