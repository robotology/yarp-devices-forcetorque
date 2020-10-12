# Copyright: (C) 2016 iCub Facility - Fondazione Istituto Italiano di Tecnologia
# Authors: Silvio Traversaro <silvio.traversaro@iit.it>, Francisco Andrade <franciscojavier.andradechavez@iit.it>
# CopyPolicy: Released under the terms of the GNU LGPL v2.0+

cmake_minimum_required(VERSION 2.8.9)
# Add cmake policy to resolve usage of <packagename>_ROOT
# Reference: https://cmake.org/cmake/help/latest/policy/CMP0074.html#policy:CMP0074
cmake_policy(SET CMP0074 NEW)
project(forcetorque-yarp-devices)

# Add local CMake files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Find YARP and include the CMake code to compile plugins
find_package(YARP REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${YARP_MODULE_PATH})
include(YarpPlugin)
include(YarpInstallationHelpers)
set(YARP_FORCE_DYNAMIC_PLUGINS TRUE)

add_subdirectory(amti)
add_subdirectory(optoforce)
add_subdirectory(ATI_Ethernet)
add_subdirectory(ftNode)
add_subdirectory(ftShoe)
add_subdirectory(ftShoeUdpWrapper)
