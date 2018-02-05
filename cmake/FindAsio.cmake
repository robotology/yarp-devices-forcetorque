#.rst:
# FindAsio
# -----------
#
# Find the Asio library.
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines the following :prop_tgt:`IMPORTED` targets if
# Asio has been found::
#
#   Asio::Asio
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables::
#
#   Asio_FOUND                - System has Asio
#   Asio_INCLUDE_DIRS         - Include directories for Asio
#   Asio_LIBRARIES            - imported targets to link against Asio
#
# Readed enviromental variables
# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#
# This module reads hints about search locations from variables::
#
#   Asio_ROOT                 - Directory containing the include and lib directories

#=============================================================================
# Copyright 2017  iCub Facility, Istituto Italiano di Tecnologia
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

find_path(Asio_INCLUDE_DIR asio.hpp
          HINTS $ENV{Asio_ROOT})

mark_as_advanced(Asio_INCLUDE_DIR)

if(Asio_INCLUDE_DIR AND NOT TARGET Asio::Asio)
  add_library(Asio::Asio UNKNOWN IMPORTED)
  set_target_properties(Asio::Asio PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${Asio_INCLUDE_DIR}")

  set(Asio_LIBRARIES Asio::Asio)
  set(Asio_INCLUDE_DIRS "${Asio_INCLUDE_DIR}")
endif()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Asio
                                  FOUND_VAR Asio_FOUND
                                  REQUIRED_VARS Asio_INCLUDE_DIRS)

# Set package properties if FeatureSummary was included
if(COMMAND set_package_properties)
  set_package_properties(Asio PROPERTIES DESCRIPTION "Asio Library"
                                         URL "https://think-async.com/")
endif()
