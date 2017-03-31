# Copyright: (C) 2016 iCub Facility - Fondazione Istituto Italiano di Tecnologia
# Authors: Silvio Traversaro <silvio.traversaro@iit.it>, Francisco Andrade <franciscojavier.andradechavez@iit.it>
# CopyPolicy: Released under the terms of the GNU LGPL v2.0+

cmake_minimum_required(VERSION 2.8.9)
project(forcetorque-yarp-devices)

# Add local CMake files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Find YARP and include the CMake code to compile plugins
find_package(YARP REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${YARP_MODULE_PATH})
include(YarpPlugin)
include(YarpInstallationHelpers)
set(YARP_FORCE_DYNAMIC_PLUGINS TRUE)
yarp_configure_external_installation(${PROJECT_NAME} WITH_PLUGINS)

#add_subdirectory(forcetorqueDriverExample)
add_subdirectory(optoforce)
add_subdirectory(ATI_Ethernet)
add_subdirectory(AME)
