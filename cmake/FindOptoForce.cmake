#.rst:
# FindOptoForce
# -----------
#
# Find the OptoForce library API.
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines the following :prop_tgt:`IMPORTED` targets if
# OptoForce has been found::
#
#   OptoForce::OMD
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables::
#
#   OptoForce_FOUND                - System has OptoFoce
#   OptoForce_INCLUDE_DIRS         - Include directories for OptoForce
#   OptoForce_LIBRARIES            - libraries to link against OptoForce
#
# Read (enviromental) variables
# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#
# This module reads hints about search locations from the following (enviromental) variables::
#
#   OptoForce_ROOT                 - Directory containing the include and lib directories 
#
#
#=============================================================================
# Copyright 2016  iCub Facility, Istituto Italiano di Tecnologia
#   Authors: Silvio Traversaro <silvio.traversaro@iit.it>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of YCM, substitute the full
#  License text for the above reference.)

if(WIN32)
    message(WARNING "At the moment, FindOptoForce does not support Windows.")
    set(OptoForce_FOUND FALSE)
else()
    find_path(OptoForce_INCLUDE_DIRS omd/opto.h
              HINTS $ENV{OptoForce_ROOT}/include
                    ${OptoForce_ROOT}/include)
    
    find_library(OMD_LIBRARY OMD
                 HINTS $ENV{OptoForce_ROOT}/lib
                       ${OptoForce_ROOT}/lib)
    
    if( OMD_LIBRARY AND OptoForce_INCLUDE_DIRS )
        add_library(OptoForce::OMD UNKNOWN IMPORTED)
        set_property(TARGET OptoForce::OMD PROPERTY IMPORTED_LOCATION ${OMD_LIBRARY})
        set_property(TARGET OptoForce::OMD APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${OptoForce_INCLUDE_DIRS})
        set(OptoForce_LIBRARIES OptoForce::OMD)
        set(OptoForce_FOUND TRUE)
    endif()             
endif()

mark_as_advanced(OptoForce_INCLUDE_DIRS
                 OptoForce_LIBRARIES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OptoForce DEFAULT_MSG OptoForce_LIBRARIES)

# Set package properties if FeatureSummary was included
if(COMMAND set_package_properties)
    set_package_properties(OptoForce PROPERTIES DESCRIPTION "API for OptoForce Force Torque sensors"
                                                URL "http://optoforce.com/support/")
endif()
