# Copyright: (C) 2016 Fondazione Istituto Italiano di Tecnologia
# Authors: Francesco Romano
# CopyPolicy: Released under the terms of the GNU LGPL v2.1+

# Compile the plugins by default
set(COMPILE_BY_DEFAULT ON)

YARP_PREPARE_PLUGIN(amtiplatforms TYPE yarp::dev::AMTIPlatformsDriver
                                  INCLUDE include/AMTIPlatformsDriver.h
				  CATEGORY device)

if(ENABLE_amtiplatforms)

    find_package(AMTIDriver REQUIRED)

    if (CMAKE_VERSION VERSION_LESS "3.1")
        set(CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_STANDARD 11)
    endif()

    include_directories(include)
    include_directories(SYSTEM ${YARP_INCLUDE_DIRS})

    yarp_add_plugin(amtiplatforms src/AMTIPlatformsDriver.cpp
                                  src/AMTIlib.cpp
                                  src/IMultipleForcePlates.cpp
                                  include/AMTIPlatformsDriver.h
                                  include/AMTIlib.h
                                  include/IMultipleForcePlates.h)

    target_link_libraries(amtiplatforms YARP::YARP_OS YARP::YARP_dev YARP::YARP_sig AMTI::USBDevice)

    yarp_install(TARGETS amtiplatforms
                 COMPONENT runtime
                 LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
                 ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR})

    yarp_install(FILES amtiplatforms.ini  DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})

	YARP_PREPARE_PLUGIN(amtiforceplate TYPE yarp::dev::AMTIForcePlate
                                           INCLUDE AMTIForcePlate.h
					   CATEGORY device
				           EXTRA_CONFIG WRAPPER=AnalogServer)
    if(ENABLE_amtiforceplate)

        add_compile_definitions(_USE_MATH_DEFINES) #For using M_PI macro
        find_package(iDynTree REQUIRED)
        find_package(Eigen3 REQUIRED)

        yarp_add_plugin(amtiforceplate src/IMultipleForcePlates.cpp
                                       src/AMTIForcePlate.cpp
                                       include/IMultipleForcePlates.h
                                       include/AMTIForcePlate.h)

        target_include_directories(amtiforceplate PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                                  SYSTEM PRIVATE ${EIGEN3_INCLUDE_DIR})

        target_link_libraries(amtiforceplate YARP::YARP_OS YARP::YARP_dev YARP::YARP_sig YARP::YARP_math)

        yarp_install(TARGETS amtiforceplate
                     COMPONENT runtime
                     LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
                     ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR})

        yarp_install(FILES amtiforceplate.ini  DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})

    endif()

endif()
