# Copyright: (C) 2016 Fondazione Istituto Italiano di Tecnologia
# Authors: Silvio Traversaro
# CopyPolicy: Released under the terms of the GNU LGPL v2.1+

# Compile the plugins by default
set(COMPILE_BY_DEFAULT ON)
YARP_PREPARE_PLUGIN(optoforce TYPE yarp::dev::optoforceDriver
                              INCLUDE optoforceDriver.h
							  CATEGORY device
                              EXTRA_CONFIG WRAPPER=AnalogServer)

if(ENABLE_optoforce)

    yarp_add_plugin(optoforce optoforceDriver.cpp optoforceDriver.h)

    find_package(OptoForce REQUIRED)

    include_directories(${YARP_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(optoforce ${OptoForce_LIBRARIES})
    yarp_install(TARGETS optoforce
                 COMPONENT runtime
                 LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
                 ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR})

    yarp_install(FILES optoforce.ini  DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})

endif()
