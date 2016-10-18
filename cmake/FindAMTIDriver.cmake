#.rst:
# FindAMTIDriver
# -----------
#
# Find the AMTI library API.
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines the following :prop_tgt:`IMPORTED` targets if
# AMTI drivers has been found::
#
#   AMTI::USBDevice
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables::
#
#   AMTIDriver_FOUND                - System has AMTI
#   AMTIDriver_INCLUDE_DIRS         - Include directories for AMTI
#   AMTIDriver_LIBRARIES            - libraries to link against AMTI
#
# Readed enviromental variables
# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#
# This module reads hints about search locations from variables::
#
#   AMTIDriver_ROOT                 - Directory containing the include and lib directories 

#=============================================================================
# Copyright 2016  iCub Facility, Istituto Italiano di Tecnologia
#   Authors: Francesco Romano <francesco.romano@iit.it>
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
    find_path(AMTI_USBDevice_INCLUDE_DIR AMTIUSBDeviceDefinitions.h
              HINTS $ENV{AMTIDriver_ROOT})
    
    find_library(AMTI_USBDevice_LIBRARY AMTIUSBDevice
                 HINTS $ENV{AMTIDriver_ROOT})

    mark_as_advanced(AMTI_USBDevice_INCLUDE_DIR
                     AMTI_USBDevice_LIBRARY)

    if( AMTI_USBDevice_LIBRARY AND AMTI_USBDevice_INCLUDE_DIR AND NOT TARGET AMTI::USBDevice)
        add_library(AMTI::USBDevice UNKNOWN IMPORTED)

		set_target_properties(AMTI::USBDevice PROPERTIES
											  INTERFACE_INCLUDE_DIRECTORIES "${AMTI_USBDevice_INCLUDE_DIR}"
											  IMPORTED_LINK_INTERFACE_LANGUAGES "C"
											  IMPORTED_LOCATION "${AMTI_USBDevice_LIBRARY}")

        set(AMTIDriver_LIBRARIES AMTI::USBDevice)
		set(AMTIDriver_INCLUDE_DIRS "${AMTI_USBDevice_INCLUDE_DIR}")
    endif()    
else()
	message(WARNING "At the moment, FindAMTIDriver supports only Windows.")
    set(AMTIDriver_FOUND FALSE)         
endif()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AMTIDriver
                                  FOUND_VAR AMTIDriver_FOUND
								  REQUIRED_VARS AMTIDriver_LIBRARIES AMTIDriver_INCLUDE_DIRS)

# Set package properties if FeatureSummary was included
if(COMMAND set_package_properties)
    set_package_properties(AMTIDriver PROPERTIES DESCRIPTION "API for AMTI Force Plate sensors"
                                                 URL "http://www.amti.biz")
endif()
