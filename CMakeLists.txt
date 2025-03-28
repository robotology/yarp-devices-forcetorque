# Copyright: (C) 2016 iCub Facility - Fondazione Istituto Italiano di Tecnologia
# Authors: Silvio Traversaro <silvio.traversaro@iit.it>, Francisco Andrade <franciscojavier.andradechavez@iit.it>
# CopyPolicy: Released under the terms of the GNU LGPL v2.0+

cmake_minimum_required(VERSION 3.16)
project(forcetorque-yarp-devices)

# Add local CMake files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Find YARP and include the CMake code to compile plugins
find_package(YARP REQUIRED)

set(YARP_FORCE_DYNAMIC_PLUGINS TRUE)

# Warning: the <package> option of yarp_configure_plugins_installation should be different from the plugin names
yarp_configure_plugins_installation(forcetorque_devices_lookup)

add_subdirectory(amti)
add_subdirectory(optoforce)
add_subdirectory(ATI_Ethernet)
add_subdirectory(ftNode)
add_subdirectory(ftShoe)
add_subdirectory(ftShoeUdpWrapper)
